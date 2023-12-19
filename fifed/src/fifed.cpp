#include "fifed.hpp"
#include "editor_module.hpp"

#include "fif/fif.hpp"
#include "fif/native_scripting/native_scripting_module.hpp"

static const ApplicationProperties APP_PROPS = {
	.windowProps =
		{
#ifdef FIF_DEBUG
			.title = "Fif Editor [DEBUG]",
#else
			.title = "Fif Editor",
#endif
			.size = fif::i16vec2(1280, 720),
			.iconPath = "assets/textures/logo.png",
			.vsync = false,
		},

	.createDefaultScene = true,
};

namespace fifed {
	Fifed::Fifed() : Application(APP_PROPS) {
	}

	void Fifed::setup_modules() {
		attach_module<LuaScriptingModule>();
		attach_module<NativeScriptingModule>();
		attach_module<InputModule>();
		attach_module<GfxModule>();
		attach_module<EditorModule>();
		attach_module<ImGuiModule>();
	}
}// namespace fifed

Application *get_application() {
	return reinterpret_cast<Application *>(new fifed::Fifed());
}
