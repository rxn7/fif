#include "fif/lua_scripting/lua_scripting_module.hpp"

#include <sol/protected_function_result.hpp>

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

		m_Lua.set_function("log", [](const std::string &msg) { core::Logger::info("[LUA] %s", msg.c_str()); });
		m_Lua.set_function("log_error", [](const std::string &msg) { core::Logger::error("[LUA] %s", msg.c_str()); });
		m_Lua.set_function("log_warn", [](const std::string &msg) { core::Logger::warn("[LUA] %s", msg.c_str()); });

		m_Lua.set_function("get_component", [](const std::string &msg) { core::Logger::info("[LUA] %s", msg.c_str()); });
	}

	void LuaScriptingModule::attach_script(LuaScriptComponent &script, const std::string &path) {
		sol::protected_function_result result = m_Lua.safe_script_file(path);
		if(!result.valid()) {
			script.loaded = false;
			script.updateFunc = nullptr;
			script.renderFunc = nullptr;

			const sol::error error = result;
			core::Logger::error("Failed to load lua script: %s", error.what());

			return;
		}

		script.path = path;
		script.loaded = true;
		script.updateFunc = m_Lua["Update"];
		script.renderFunc = m_Lua["Render"];
	}

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.updateFunc.valid())
				luaScript.updateFunc(dt);
		});
	}

	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.renderFunc.valid())
				luaScript.renderFunc();
		});
	}

}// namespace fif::lua_scripting
