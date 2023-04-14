#include "fif/core/application.h"
#include "fif/core/assertion.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

fif::core::Application *fif::core::Application::s_Instance = nullptr;

fif::core::Application::Application(const WindowProperties &windowProperties) {
	FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
	s_Instance = this;

	mp_Window = std::make_unique<Window>(windowProperties);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
}

fif::core::Application::~Application() {
}

void fif::core::Application::start() {
	while(!mp_Window->shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT);
		mp_Window->startFrame();

		mp_Window->endFrame();
	}
}
