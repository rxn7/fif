#include "./lua_scripting_entity_serializer.hpp"

#include "ecs/components/tag_component.hpp"
#include "ecs/components/uuid_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/core/project.hpp"
#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "fif/lua_scripting/lua_scripting_module.hpp"

namespace fif::lua_scripting {
	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, const f32 dt);
	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry);

	LuaScriptingModule::LuaScriptingModule() : m_StartCallback(std::bind(&LuaScriptingModule::on_start, this)) {
		FIF_MODULE_INIT();
		core::Application::get_instance().m_StartHook.hook(m_StartCallback);
	}

	LuaScriptingModule::~LuaScriptingModule() { core::Application::get_instance().m_StartHook.unhook(m_StartCallback); }

	void LuaScriptingModule::on_start() {
		mp_Application->add_update_system(lua_script_update_system);
		mp_Application->add_render_system(lua_script_render_system);
		core::SceneSerializer::add_serializer<LuaScriptingEntitySerializer>();

		m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

		sol::table loggerTable = m_Lua.create_named_table("Logger");
		loggerTable.set_function("info", [](const std::string &msg) { core::Logger::info("%s", msg.c_str()); });
		loggerTable.set_function("error", [](const std::string &msg) { core::Logger::error("%s", msg.c_str()); });
		loggerTable.set_function("warn", [](const std::string &msg) { core::Logger::warn("%s", msg.c_str()); });
		loggerTable.set_function("debug", [](const std::string &msg) { core::Logger::debug("%s", msg.c_str()); });

		m_Lua.new_usertype<core::Entity>("Entity", "id", sol::readonly_property(&core::Entity::m_ID));

		// clang-format off
		m_Lua.new_usertype<vec2>(
			"Vec2", sol::call_constructor, sol::factories([](f32 x, f32 y) { return vec2(x, y); }), 
			"x", &vec2::x, 
			"y", &vec2::y, 
			sol::meta_function::addition, [](const vec2 &a, const vec2 &b) { return vec2(a.x + b.x, a.y + b.y); }, 
			sol::meta_function::multiplication, [](const vec2 &a, float v) { return vec2(a.x * v, a.y * v); },
			sol::meta_function::division, [](const vec2 &a, float v) { return vec2(a.x / v, a.y / v); }, 
			sol::meta_function::subtraction, [](const vec2 &a, const vec2 &b) { return vec2(a.x - b.x, a.y - b.y); },

			"normalize", [](vec2 &v) { 
				// NOTE: Without the check glm tries to divide by 0
				if(v.x != 0 && v.y != 0)
					v = glm::normalize(v);
			},

			sol::meta_function::to_string, [](const vec2 &v) { 
				std::ostringstream ss;
				ss << "(" << v.x << ", " << v.y << ")";
				return ss.str(); 
			}
		);
		// clang-format on

		register_component<core::TransformComponent>("position", &core::TransformComponent::position, "scale", &core::TransformComponent::scale, "angleRadians", &core::TransformComponent::angleRadians, "get_right_dir", &core::TransformComponent::get_right_dir, "get_up_dir", &core::TransformComponent::get_up_dir);
		register_component<core::TagComponent>("tag", &core::TagComponent::tag);
		register_component<core::UuidComponent>("uuid", &core::UuidComponent::uuid);
	}

	void LuaScriptingModule::attach_script(core::Entity &ent, const std::string &path) {
		LuaScriptComponent &script = ent.require_component<LuaScriptComponent>(ent, path);
		init_script(script);
	}

	void LuaScriptingModule::init_script(LuaScriptComponent &luaScript) {
		luaScript.inited = false;

		std::filesystem::path path = core::Project::get_resource_path(luaScript.path);
		if(!std::filesystem::exists(path)) {
			core::Logger::error("Failed to load lua script '%s': file does not exist", path.string().c_str());
			return;
		}

		if(path.extension() != ".lua") {
			core::Logger::error("Failed to load lua script '%s': file has invalid extension", path.string().c_str());
			return;
		}

		sol::protected_function_result result = m_Lua.safe_script_file(path.string());
		if(!result.valid()) {
			sol::error err = result;
			core::Logger::error("Failed to load lua script '%s': %s", luaScript.path.c_str(), err.what());
			return;
		}

		try {
			luaScript.self = result;
		} catch(const std::exception &ex) {
			core::Logger::error("Failed to init lua script file: %s", ex.what());
			return;
		}

		luaScript.inited = true;
		luaScript.start_hook = luaScript.self["start"];
		luaScript.update_hook = luaScript.self["update"];
		luaScript.render_hook = luaScript.self["render"];

		luaScript.self["entity"] = &luaScript.entity;

		auto init = luaScript.self["init"];
		if(init.valid()) {
			result = init(luaScript.self);
			if(!result.valid()) {
				sol::error err(result);
				core::Logger::error("Failed to init lua script(%s): %s", luaScript.path.c_str(), err.what());
			}
		}
	}

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, const f32 dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.firstFrame) {
				luaScript.firstFrame = false;

				if(luaScript.start_hook.valid()) {
					const auto &result = luaScript.start_hook(luaScript.self);
					if(!result.valid()) {
						sol::error err(result);
						core::Logger::error("Failed to run start hook in lua script(%s): %s", luaScript.path.c_str(), err.what());
					}
				}
			}

			if(luaScript.update_hook.valid()) {
				const auto &result = luaScript.update_hook(luaScript.self, dt);
				if(!result.valid()) {
					sol::error err(result);
					core::Logger::error("Failed to run update hook in lua script(%s): %s", luaScript.path.c_str(), err.what());
				}
			}
		});
	}

	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.render_hook.valid()) {
				const auto result = luaScript.render_hook(luaScript.self);
				if(!result.valid()) {
					sol::error err(result);
					core::Logger::error("Failed to run render hook in lua script(%s): %s", luaScript.path.c_str(), err.what());
				}
			}
		});
	}
}// namespace fif::lua_scripting
