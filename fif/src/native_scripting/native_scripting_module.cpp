#include "fif/native_scripting/native_scripting_module.hpp"
#include "fif/core/module.hpp"

namespace fif::native_scripting {
	static void native_scripting_update_system(const core::ApplicationStatus &status, entt::registry &registry, const f32 dt) {
		if(status.paused)
			return;

		registry.view<NativeScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_update(dt); });
	}

	static void native_scripting_render_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		registry.view<NativeScriptComponent>().each([&]([[maybe_unused]] core::EntityID entity, NativeScriptComponent &script) { script.p_script->on_render(); });
	}

	NativeScriptingModule::NativeScriptingModule() {
		FIF_MODULE_INIT()
		core::Application::get_instance().add_render_system(native_scripting_render_system);
		core::Application::get_instance().add_update_system(native_scripting_update_system);
	}
}// namespace fif::native_scripting
