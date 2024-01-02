#pragma once

#include "fif/core/module.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"
#include "util/logger.hpp"

namespace fif::lua_scripting {
	class LuaScriptingModule final : public core::Module {
	public:
		FIF_MODULE(LuaScriptingModule)

		LuaScriptingModule();
		~LuaScriptingModule();

		template<typename T, typename... Args> void register_component(std::string_view name, Args &&...args) {
			m_Lua.new_usertype<T>(name, std::forward<Args>(args)...);

			std::string nameSnakeCase;
			nameSnakeCase += std::tolower(name[0]);
			for(u8 i = 1; i < name.length(); ++i) {
				const char c = name[i];
				if(std::isupper(c)) {
					nameSnakeCase += '_';
					nameSnakeCase += std::tolower(c);
				} else {
					nameSnakeCase += c;
				}
			}

			m_Lua["Entity"]["get_" + nameSnakeCase] = &core::Entity::get_component<T>;
			m_Lua["Entity"]["add_" + nameSnakeCase] = &core::Entity::add_component<T>;
			m_Lua["Entity"]["has_" + nameSnakeCase] = &core::Entity::has_component<T>;
			m_Lua["Entity"]["remove_" + nameSnakeCase] = &core::Entity::remove_component<T>;
			m_Lua["Entity"]["require_" + nameSnakeCase] = &core::Entity::require_component<T>;
		}

		inline u32 get_lua_memory_usage() const {
			return m_Lua.memory_used();
		}
		void attach_script(core::Entity &ent, const std::string &path);
		void init_script(LuaScriptComponent &luaScript);

	protected:
		void on_start() override;

	public:
		sol::state m_Lua;
	};
}// namespace fif::lua_scripting
