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
		}

		inline u32 get_lua_memory_usage() const { return m_Lua.memory_used(); }

		void on_start(core::Application &app) override;
		void attach_script(core::EntityID ent, core::Scene &scene, const std::filesystem::path &filepath);
		void run_script(const std::string &path);

	private:
		sol::state m_Lua;
	};
}// namespace fif::lua_scripting
