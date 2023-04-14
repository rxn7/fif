#include "fif/core/application.h"
#include "fif/core/window.h"
#include "fif/gfx/primitives/circle.h"
#include "fif/gfx/primitives/quad.h"
#include "fif/gfx/renderable.h"
#include "fif/gfx/vertex.h"
#include "fif/gfx/gfx.h"

constexpr fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fif Example",
	.size = glm::i16vec2(420, 420)
};

class ExampleApplication : public fif::core::Application {
public:
	ExampleApplication() : fif::core::Application(WINDOW_PROPS) {
		fif::gfx::init();
		mp_Circle = std::make_unique<fif::gfx::primitives::CirclePrimitive>();
		mp_Quad = std::make_unique<fif::gfx::primitives::QuadPrimitive>();
	}

	void startFrame() override {
		fif::core::Application::startFrame();
		mp_Quad->render();
		mp_Circle->render();
	}

	void endFrame() override {
		fif::core::Application::endFrame();
	}

private:
	std::unique_ptr<fif::gfx::Renderable> mp_Circle, mp_Quad;
};

int main() {
	ExampleApplication app;
	app.start();
}
