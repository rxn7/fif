#include "native_script_system.hpp"

#include "fif/native_scripting/components/native_script_component.hpp"

namespace fif::native_scripting {
	void native_scripting_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<NativeScriptComponent>().each(
			[&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_update(dt); });
	}

	void native_scripting_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<NativeScriptComponent>().each(
			[&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_render(); });
	}
}// namespace fif::native_scripting
