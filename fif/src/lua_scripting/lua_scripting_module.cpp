#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/gfx_module.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "util/logger.hpp"

namespace fif::lua_scripting {
	FIF_MODULE_INSTANCE_IMPL(LuaScriptingModule);

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt);
	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry);

	LuaScriptingModule::LuaScriptingModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	LuaScriptingModule::~LuaScriptingModule() {}

	void LuaScriptingModule::on_start(core::Application &app) {
		app.add_update_system(lua_script_update_system);
		app.add_render_system(lua_script_render_system);

		m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

		sol::table loggerTable = m_Lua.create_named_table("Logger");
		loggerTable.set_function("info", [](const std::string &msg) { core::Logger::info("%s", msg.c_str()); });
		loggerTable.set_function("error", [](const std::string &msg) { core::Logger::error("%s", msg.c_str()); });
		loggerTable.set_function("warn", [](const std::string &msg) { core::Logger::warn("%s", msg.c_str()); });
		loggerTable.set_function("debug", [](const std::string &msg) { core::Logger::debug("%s", msg.c_str()); });

		m_EntityUsertype = m_Lua.new_usertype<core::Entity>("Entity");
		m_EntityUsertype.set("id", sol::readonly_property(&core::Entity::get_id));

		m_Lua.new_usertype<vec2>("Vec2", "x", &vec2::x, "y", &vec2::y);

		if(gfx::GfxModule::exists()) {
			m_Lua.new_usertype<gfx::Color>("Color", "r", &gfx::Color::r, "g", &gfx::Color::g, "b", &gfx::Color::b, "a", &gfx::Color::a);
			register_component<gfx::SpriteComponent>("SpriteComponent", "tint", &gfx::SpriteComponent::tint, "size", &gfx::SpriteComponent::size);
		}

		register_component<core::TransformComponent>("TransformComponent", "position", &core::TransformComponent::position, "scale", &core::TransformComponent::scale, "angleRadians", &core::TransformComponent::angleRadians);
	}

	void LuaScriptingModule::attach_script(core::EntityID ent, core::Scene &scene, const std::filesystem::path &filepath) {
		const sol::protected_function_result &result = m_Lua.safe_script_file(filepath.string());
		if(!result.valid()) {
			sol::error err = result;
			core::Logger::error("Failed to load lua script '%s': %s", filepath.stem().c_str(), err.what());
			return;
		}

		auto &script = scene.add_component<LuaScriptComponent>(ent, result);
		script.filepath = filepath;
		script.entity = core::Entity(&scene, ent);

		// TODO: Move this to a init_script function once we have a runtime
		script.inited = true;
		script.hooks.update = script.self["update"];
		script.hooks.render = script.self["render"];

		script.self["entity"] = &script.entity;

		auto init = script.self["init"];
#ifdef FIF_DEBUG
		if(init.valid()) {
			const sol::protected_function_result result = init(script.self);
			if(!result.valid()) {
				sol::error err(result);
				core::Logger::error("Failed to init lua script(%s): %s", script.filepath.c_str(), err.what());
			}
		}
#endif
	}

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.update.valid()) {
				const auto &result = luaScript.hooks.update(luaScript.self, dt);
#ifdef FIF_DEBUG
				if(!result.valid()) {
					sol::error err(result);
					core::Logger::error("Failed to update lua script(%s): %s", luaScript.filepath.c_str(), err.what());
				}
#endif
			}
		});
	}

	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.render.valid())
				luaScript.hooks.render(luaScript.self);
		});
	}
}// namespace fif::lua_scripting
