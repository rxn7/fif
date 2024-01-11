#include "runtime.hpp"
#include "runtime_module.hpp"

static const ApplicationProperties APP_PROPS = {
	.windowProps =
		{
#ifdef FIF_DEBUG
			.title = "Fif Runtime [DEBUG]",
#else
			.title = "Fif Runtime",
#endif
			.size = fif::i16vec2(1280, 720),
			.iconPath = "icon.png",
			.vsync = false,
		},

	.createDefaultScene = true,
};

namespace fif_runtime {
	Runtime::Runtime() : Application(APP_PROPS) {}

	void Runtime::setup_modules() {
		attach_module<LuaScriptingModule>();
		attach_module<InputModule>();
		attach_module<GfxModule>("./assets/fonts/iosevka-regular.ttf");
		attach_module<RuntimeModule>();
	}
}// namespace fif_runtime

Application *get_application() { return reinterpret_cast<Application *>(new fif_runtime::Runtime()); }
