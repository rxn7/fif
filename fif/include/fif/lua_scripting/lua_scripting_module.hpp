#pragma once

#include "fif/core/module.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"

#include "sol/sol.hpp"

namespace fif::lua_scripting {
	class LuaScriptingModule final : public core::Module {
	public:
		FIF_MODULE(LuaScriptingModule)

		LuaScriptingModule();
		~LuaScriptingModule();

		void on_start(core::Application &app) override;
		void attach_script(LuaScriptComponent &component, const std::string &path);
		void run_script(const std::string &path);
	};
}// namespace fif::lua_scripting
