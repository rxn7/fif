#include "fif/native_scripting/native_scripting_module.hpp"
#include "module.hpp"

namespace fif::native_scripting {
	FIF_MODULE_INSTANCE_IMPL(NativeScriptingModule)

	NativeScriptingModule::NativeScriptingModule() {
		FIF_MODULE_INIT_INSTANCE()
	}

	static void native_scripting_update_system(const core::ApplicationStatus &status, entt::registry &registry, float dt) {
		if(status.paused)
			return;

		registry.view<NativeScriptComponent>().each(
			[&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_update(dt); });
	}

	static void native_scripting_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<NativeScriptComponent>().each(
			[&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_render(); });
	}

	void NativeScriptingModule::on_start() {
		mp_Application->add_render_system(native_scripting_render_system);
		mp_Application->add_update_system(native_scripting_update_system);
	}
}// namespace fif::native_scripting
