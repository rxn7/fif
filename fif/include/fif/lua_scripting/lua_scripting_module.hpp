#pragma once

#include "fif/core/module.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"

#include "sol/sol.hpp"

namespace fif::lua_scripting {
	class LuaScriptingModule final : public core::Module {
	public:
		FIF_MODULE_NAME(LuaScriptingModule)
		FIF_MODULE_INSTANCE_FUNC_DECL(LuaScriptingModule);

		LuaScriptingModule();
		~LuaScriptingModule();

		void on_start(core::Application &app) override;
		void attach_script(LuaScriptComponent &component, const std::string &path);
		void run_script(const std::string &path);

	private:
		sol::state m_Lua;
	};
}// namespace fif::lua_scripting
