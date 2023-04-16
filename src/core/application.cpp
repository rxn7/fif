#include "fif/core/application.h"
#include "fif/core/assertion.h"
#include "fif/core/events/updateEvent.h"
#include "fif/core/events/renderEvent.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <algorithm>
#include <chrono>

using namespace std::chrono;

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties) {
		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(windowProperties);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(mp_Window->getGlfwWindow(), true);
	}

	Application::~Application() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
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
			startFrame(dt);
			endFrame();
		}
	}

	void Application::startFrame(float dt) {
		FIF_PROFILE_FUNC();

		fif::core::Profiler::clear();
		mp_Window->startFrame();

		for(const Module * mod : m_Modules)
			mod->updateFunc(dt);

		for(std::unique_ptr<Layer> &layer : m_Layers)
			layer->update(dt);
	}

	void Application::endFrame() {
		FIF_PROFILE_FUNC();

		for(const Module *mod : m_Modules)
			mod->renderFunc();

		for(std::unique_ptr<Layer> &layer : m_Layers)
			layer->render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		for(std::unique_ptr<Layer> &layer : m_Layers)
			layer->renderImGui();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mp_Window->endFrame();
	}

	void Application::registerModule(const Module *mod) {
		mod->initFunc();
		m_Modules.push_back(mod);
		FIF_LOG("Module " << mod->name << " registered");
	}

	void Application::addLayer(std::unique_ptr<Layer> layer) {
		const auto sortFunc = [](std::unique_ptr<Layer> &a, std::unique_ptr<Layer> &b) {
			return a->getZIndex() < b->getZIndex();
		};

		m_Layers.push_back(std::move(layer));
		std::sort(m_Layers.begin(), m_Layers.end(), sortFunc);
	}
}
