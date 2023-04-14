#include "fif/core/application.h"
#include "fif/core/window.h"

int main() {
	fif::core::WindowProperties props = {
		.title = "fif Example",
		.size = glm::i16vec2(420, 420)
	};
	fif::core::Application app(props);
	app.start();
}
