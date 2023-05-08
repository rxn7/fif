#include "lua_script_system.hpp"
#include "fif/lua_scripting/components/lua_script_component.hpp"

#include "fif/core/ecs/scene.hpp"

namespace fif::lua_scripting {
	void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.updateFunc.valid())
				luaScript.updateFunc(dt);
		});
	}

	void lua_script_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<LuaScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, LuaScriptComponent &luaScript) {
			if(luaScript.renderFunc.valid())
				luaScript.renderFunc();
		});
	}
}// namespace fif::lua_scripting
