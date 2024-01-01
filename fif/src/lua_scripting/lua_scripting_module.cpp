#include "./lua_scripting_serializer.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/serialization/scene_serializer.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

namespace fif::lua_scripting {
	FIF_MODULE_INSTANCE_IMPL(LuaScriptingModule);

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt);
	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry);

	LuaScriptingModule::LuaScriptingModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	LuaScriptingModule::~LuaScriptingModule() {
	}

	void LuaScriptingModule::on_start() {
		mp_Application->add_update_system(lua_script_update_system);
		mp_Application->add_render_system(lua_script_render_system);
		core::SceneSerializer::add_serializer<LuaScriptingSerializer>();

		m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

		sol::table loggerTable = m_Lua.create_named_table("Logger");
		loggerTable.set_function("info", [](const std::string &msg) { core::Logger::info("%s", msg.c_str()); });
		loggerTable.set_function("error", [](const std::string &msg) { core::Logger::error("%s", msg.c_str()); });
		loggerTable.set_function("warn", [](const std::string &msg) { core::Logger::warn("%s", msg.c_str()); });
		loggerTable.set_function("debug", [](const std::string &msg) { core::Logger::debug("%s", msg.c_str()); });

		m_Lua.new_usertype<core::Entity>("Entity", "id", sol::readonly_property(&core::Entity::m_ID));
		m_Lua.new_usertype<vec2>("Vec2", sol::call_constructor, sol::factories([](f32 x, f32 y) { return vec2(x, y); }), "x", &vec2::x, "y", &vec2::y);

		register_component<core::TransformComponent>("TransformComponent", "position", &core::TransformComponent::position, "scale", &core::TransformComponent::scale, "angleRadians", &core::TransformComponent::angleRadians);
	}

	void LuaScriptingModule::attach_script(core::Entity &ent, const std::filesystem::path &filepath) {
		auto &script = ent.add_component<LuaScriptComponent>(core::Entity(ent.m_Scene, ent.m_ID));
		script.filepath = filepath;
		init_script(script);
	}

	void LuaScriptingModule::init_script(LuaScriptComponent &luaScript) {
		luaScript.inited = false;
		luaScript.hooks = {};
		luaScript.self = {};

		const sol::protected_function_result &result = m_Lua.safe_script_file(luaScript.filepath.string());
		if(!result.valid()) {
			sol::error err = result;
			core::Logger::error("Failed to load lua script '%s': %s", luaScript.filepath.stem().c_str(), err.what());
			return;
		}

		luaScript.self = result;

		luaScript.inited = true;
		luaScript.hooks.start = luaScript.self["start"];
		luaScript.hooks.update = luaScript.self["update"];
		luaScript.hooks.render = luaScript.self["render"];

		luaScript.self["entity"] = &luaScript.entity;

		auto init = luaScript.self["init"];
		if(init.valid()) {
			const sol::protected_function_result result = init(luaScript.self);
			if(!result.valid()) {
				sol::error err(result);
				core::Logger::error("Failed to init lua script(%s): %s", luaScript.filepath.c_str(), err.what());
			}
		}
	}

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.update.valid()) {
				if(luaScript.firstFrame) {
					luaScript.firstFrame = false;

					const auto &result = luaScript.hooks.start(luaScript.self);
					if(!result.valid()) {
						sol::error err(result);
						core::Logger::error("Failed to run start hook in lua script(%s): %s", luaScript.filepath.c_str(), err.what());
					}
				}

				const auto &result = luaScript.hooks.update(luaScript.self, dt);
				if(!result.valid()) {
					sol::error err(result);
					core::Logger::error("Failed to run update hook in lua script(%s): %s", luaScript.filepath.c_str(), err.what());
				}
			}
		});
	}

	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.render.valid()) {
				const auto &result = luaScript.hooks.render(luaScript.self);
				if(!result.valid()) {
					sol::error err(result);
					core::Logger::error("Failed to run render hook in lua script(%s): %s", luaScript.filepath.c_str(), err.what());
				}
			}
		});
	}
}// namespace fif::lua_scripting
