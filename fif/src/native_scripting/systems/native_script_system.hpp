#pragma once

namespace fif::native_scripting {
	void native_scripting_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt);
	void native_scripting_render_system(const core::ApplicationStatus &status, entt::registry &registry);
}// namespace fif::native_scripting
