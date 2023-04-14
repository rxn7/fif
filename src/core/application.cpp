#include "fif/core/application.h"
#include "fif/core/assertion.h"
#include "fif/core/layers/imGuiLayer.h"

#include "GLFW/glfw3.h"
#include "fif/core/performanceStats.h"
#include "glad/glad.h"
#include <chrono>

using namespace std::chrono;

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties) {
		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(windowProperties);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		m_Layers.push_back(std::make_unique<ImGuiLayer>());
	}

	Application::~Application() {
	}

	void Application::start() {
		auto now = high_resolution_clock::now();
		auto lastFrame = now;
		float dt = 0.0f;

		while(!mp_Window->shouldClose()) {
			now = high_resolution_clock::now();
			dt = duration_cast<microseconds>(now - lastFrame).count() * 0.000001f;
			lastFrame = now;

			m_LastFramePerformanceStats.fps = 1.0f / dt;
			m_LastFramePerformanceStats.frameTimeMs = dt * 1000.0f;

			glClear(GL_COLOR_BUFFER_BIT);
			startFrame();
			endFrame();
		}
	}

	void Application::startFrame() {
		mp_Window->startFrame();

		for(std::unique_ptr<Layer> &layer : m_Layers)
			layer->render();
	}

	void Application::endFrame() {
		mp_Window->endFrame();
	}
}
