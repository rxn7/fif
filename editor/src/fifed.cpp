#include "fifed.h"
#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/gfx/gfxModule.h"

const fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(420, 420)
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS) {
	attachModule<fif::gfx::GfxModule>();
	addLayer<EditorLayer>();
}

Fifed::~Fifed() {
}
