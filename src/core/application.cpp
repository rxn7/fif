#include "fif/core/application.h"
#include "fif/core/assertion.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties) {
		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(windowProperties);

		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	}

	Application::~Application() {
	}

	void Application::start() {
		while(!mp_Window->shouldClose()) {
			glClear(GL_COLOR_BUFFER_BIT);
			startFrame();
			endFrame();
		}
	}

	void Application::startFrame() {
		mp_Window->startFrame();
	}

	void Application::endFrame() {
		mp_Window->endFrame();
	}
}
