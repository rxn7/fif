#include "fifed.hpp"
#include "editorModule.hpp"

#include "fif/fif.hpp"
#include "fif/gfx/gfxModule.hpp"
#include "fif/imGui/imGuiModule.hpp"
#include "fif/input/inputModule.hpp"

const fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(1280, 720),
	.vsync = false,
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS, false) {
	attachModule<EditorModule>();
	attachModule<fif::input::InputModule>();
	attachModule<fif::gfx::GfxModule>();
	attachModule<fif::imgui::ImGuiModule>();
}

fif::core::Application *getApplication() {
	return new Fifed();
}
