#include "fifed.h"
#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/gfx/gfx.h"

constexpr fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(420, 420)
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS) {
	registerModule(&fif::gfx::gfxModule);
	m_Layers.push_back(std::make_unique<EditorLayer>());
}

Fifed::~Fifed() {
}
