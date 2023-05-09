#include "fif/native_scripting/native_scripting_module.hpp"
#include "module.hpp"
#include "systems/native_script_system.hpp"

namespace fif::native_scripting {
	FIF_MODULE_INSTANCE_IMPL(NativeScriptingModule)

	NativeScriptingModule::NativeScriptingModule() {
		FIF_MODULE_INIT_INSTANCE()
	}

	void NativeScriptingModule::on_start(core::Application &app) {
		app.add_render_system(native_scripting_render_system);
		app.add_update_system(native_scripting_update_system);
	}
}// namespace fif::native_scripting
