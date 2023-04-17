#include "fifed.h"
#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/gfx/gfxModule.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
#include "fif/input/inputModule.h"

const fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(1280, 720)
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS) {
	attachModule<fif::gfx::GfxModule>();
	attachModule<fif::input::InputModule>();
	addLayer<EditorLayer>();
}

Fifed::~Fifed() {
}
