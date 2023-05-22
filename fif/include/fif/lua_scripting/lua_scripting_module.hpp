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

		inline u32 get_lua_memory_usage() const { return m_Lua.memory_used(); }

		void on_start(core::Application &app) override;
		void attach_script(core::EntityID ent, core::Scene &scene, const std::filesystem::path &filepath);
		void run_script(const std::string &path);

	private:
		sol::state m_Lua;
	};
}// namespace fif::lua_scripting
