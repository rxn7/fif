#pragma once

#include "fif/core/module.hpp"
#include "fif/core/system.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"

namespace fif::lua_scripting {
	class LuaScriptingModule final : public core::Module {
	public:
		FIF_MODULE(LuaScriptingModule)

		LuaScriptingModule();
		~LuaScriptingModule();

		void attach_script(core::Entity &ent, const std::filesystem::path &path);
		void init_script(LuaScriptComponent &luaScript);

		template<typename T, typename... Args> void register_component(Args &&...args) {
			std::string name = core::System::get_type_name<T>();
			name = name.substr(name.find_last_of(':') + 1);// Remove the namespaces

			m_Lua.new_usertype<T>(name, std::forward<Args>(args)...);

			const std::string nameSnakeCase = core::System::camel_case_to_snake_case(name);

			core::Logger::debug("Lua component registered: %s", nameSnakeCase.c_str());

			m_Lua["Entity"]["get_" + nameSnakeCase] = &core::Entity::get_component<T>;
			m_Lua["Entity"]["add_" + nameSnakeCase] = &core::Entity::add_component<T>;
			m_Lua["Entity"]["has_" + nameSnakeCase] = &core::Entity::has_component<T>;
			m_Lua["Entity"]["remove_" + nameSnakeCase] = &core::Entity::remove_component<T>;
			m_Lua["Entity"]["require_" + nameSnakeCase] = &core::Entity::require_component<T>;
		}

		inline u32 get_lua_memory_usage() const { return m_Lua.memory_used(); }

	private:
		void on_start();

	public:
		sol::state m_Lua;

	private:
		Callback<> m_StartCallback;
	};
}// namespace fif::lua_scripting
