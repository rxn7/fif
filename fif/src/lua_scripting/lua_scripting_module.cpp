#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "systems/lua_script_system.hpp"

#include <sol/protected_function_result.hpp>

namespace fif::lua_scripting {
	FIF_MODULE_INSTANCE_IMPL(LuaScriptingModule);

	LuaScriptingModule::LuaScriptingModule() {
		FIF_MODULE_INIT_INSTANCE();
		m_Lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

		const auto log = [](const std::string &msg) { FIF_LOG("[LUA] " << msg); };
		m_Lua.set_function("log", log);

		const auto log_error = [](const std::string &msg) { FIF_LOG_ERROR("[LUA] " << msg); };
		m_Lua.set_function("log_error", log_error);

		const auto log_warn = [](const std::string &msg) { FIF_LOG_WARNING("[LUA] " << msg); };
		m_Lua.set_function("log_warn", log_warn);
	}

	LuaScriptingModule::~LuaScriptingModule() {}

	void LuaScriptingModule::on_start(core::Application &app) {
		app.add_update_system(lua_script_update_system);
		app.add_render_system(lua_script_render_system);
	}

	void LuaScriptingModule::attach_script(LuaScriptComponent &script, const std::string &path) {
		sol::protected_function_result result = m_Lua.safe_script_file(path);
		if(!result.valid()) {
			const sol::error error = result;
			FIF_LOG_ERROR("Failed to load lua script: " << error.what());
			script.loaded = false;
			script.updateFunc = nullptr;
			script.renderFunc = nullptr;
			return;
		}

		script.path = path;
		script.loaded = true;
		script.updateFunc = m_Lua["Update"];
		script.renderFunc = m_Lua["Render"];
	}

	void LuaScriptingModule::run_script(const std::string &path) {
		sol::protected_function_result result = m_Lua.safe_script_file(path);
		if(!result.valid()) {
			const sol::error error = result;
			FIF_LOG_ERROR("Failed to load lua script: " << error.what());
		}
	}

}// namespace fif::lua_scripting
