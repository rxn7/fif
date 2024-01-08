#include "fifed.hpp"
#include "fifed_module.hpp"

#include <fif/core/project.hpp>
#include <fif/fif.hpp>

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
	Fifed::Fifed() : Application(APP_PROPS) {}

	void Fifed::setup_modules() {
		attach_module<LuaScriptingModule>();
		attach_module<InputModule>();
		attach_module<GfxModule>("./assets/fonts/iosevka-regular.ttf");
		attach_module<FifedModule>();
		attach_module<ImGuiModule>();
	}
}// namespace fifed

Application *get_application() { return reinterpret_cast<Application *>(new fifed::Fifed()); }
