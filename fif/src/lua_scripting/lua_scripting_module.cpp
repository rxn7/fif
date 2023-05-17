#include "fif/lua_scripting/lua_scripting_module.hpp"
#include "components/lua_script_component.hpp"
#include "ecs/scene.hpp"
#include <sol/property.hpp>

namespace fif::lua_scripting {
	FIF_MODULE_INSTANCE_IMPL(LuaScriptingModule);

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt);
	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry);

	inline static void on_lua_panic(sol::optional<std::string> msg) {
		if(msg)
			core::Logger::error("Lua panic: %s", msg.value());
	}

	LuaScriptingModule::LuaScriptingModule() : m_Lua(sol::c_call<decltype(&on_lua_panic), &on_lua_panic>) {
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
	}

	void LuaScriptingModule::attach_script(core::EntityID ent, core::Scene &scene, const std::string &path) {
		auto result = m_Lua.script_file(path);

		if(!result.valid()) {
			sol::error err = result;
			core::Logger::error("Failed to load lua script '%s': %s", path.c_str(), err.what());
			return;
		}

		auto &script = scene.add_component<LuaScriptComponent>(ent, result);
		script.path = path;

		// TODO: Move this to a init_script function once we have a runtime
		script.hooks.update = script.self["update"];
		script.hooks.render = script.self["render"];

		script.self["id"] = sol::readonly_property([ent] { return ent; });

		auto init = script.self["init"];
		if(init.valid())
			init(script.self);
	}

	static void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.update.valid())
				luaScript.hooks.update(luaScript.self, dt);
		});
	}

	static void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.hooks.render.valid())
				luaScript.hooks.render(luaScript.self);
		});
	}
}// namespace fif::lua_scripting
