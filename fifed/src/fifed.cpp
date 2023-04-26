#include "fifed.hpp"
#include "editor_module.hpp"

#include "fif/fif.hpp"
#include "fif/gfx/gfx_module.hpp"
#include "fif/imgui/imgui_module.hpp"
#include "fif/input/input_module.hpp"

const fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(1280, 720),
	.vsync = false,
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS, false) {
	attach_module<fif::input::InputModule>();
	attach_module<fif::gfx::GfxModule>();
	attach_module<EditorModule>();
	attach_module<fif::imgui::ImGuiModule>();
}

fif::core::Application *get_application() {
	return new Fifed();
}
