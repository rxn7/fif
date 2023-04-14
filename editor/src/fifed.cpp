#include "fifed.h"
#include "fif/core/application.h"

constexpr fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fifed",
	.size = glm::i16vec2(420, 420)
};

Fifed::Fifed() : fif::core::Application(WINDOW_PROPS) {
}

Fifed::~Fifed() {
}
