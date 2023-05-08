#pragma once

namespace fif::lua_scripting {
	void lua_script_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt);
	void lua_script_render_system(const core::ApplicationStatus &status, entt::registry &registry);
}// namespace fif::lua_scripting
