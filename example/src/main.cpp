#include "fif/core/application.h"
#include "fif/core/window.h"
#include "fif/gfx/renderable.h"
#include "fif/gfx/vertex.h"

constexpr fif::core::WindowProperties WINDOW_PROPS = {
	.title = "fif Example",
	.size = glm::i16vec2(420, 420)
};

class ExampleApplication : public fif::core::Application {
public:
	ExampleApplication() : fif::core::Application(WINDOW_PROPS) {
		std::vector<fif::gfx::Vertex> vertices = {
			fif::gfx::Vertex{
				.position = glm::vec3(-0.5f, -0.5f, 0.0f),
				.uv = {0.0f, 0.0f},
				.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			},
			fif::gfx::Vertex{
				.position = glm::vec3( 0.5f, -0.5f, 0.0f),
				.uv = {1.0f, 0.0f},
				.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			},
			fif::gfx::Vertex{
				.position = glm::vec3( 0.0f,  0.5f, 0.0f),
				.uv = {0.0f, 1.0f},
				.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
			},
		};

		std::vector<std::uint32_t> elements = {0,1,2};

		mp_Triangle = std::make_unique<fif::gfx::Renderable>(vertices, elements);
	}

	void startFrame() override {
		fif::core::Application::startFrame();
		mp_Triangle->render();
	}

	void endFrame() override {
		fif::core::Application::endFrame();
	}

private:
	std::unique_ptr<fif::gfx::Renderable> mp_Triangle;
};

int main() {
	ExampleApplication app;
	app.start();
}
