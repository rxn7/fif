#include "fifed.h"
#include "editorModule.h"

#include "fif/core/application.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/gfx/gfxModule.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
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
