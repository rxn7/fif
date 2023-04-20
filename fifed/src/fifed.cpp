#include "fifed.h"
#include "editorModule.h"

#include "fif/fif.h"
#include "fif/gfx/gfxModule.h"
#include "fif/imGui/imGuiModule.h"
#include "fif/input/inputModule.h"

const fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(1280, 720),
	.vsync = false,
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS) {
	attachModule<EditorModule>();
	attachModule<fif::input::InputModule>();
	attachModule<fif::gfx::GfxModule>();
	attachModule<fif::imgui::ImGuiModule>();
}

fif::core::Application *getApplication() {
	return new Fifed();
}
