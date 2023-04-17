#include "fif/core/application.h"
#include "GLFW/glfw3.h"
#include "fif/core/assertion.h"
#include "fif/core/clock.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/windowEvent.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"

#include "fif/core/opengl.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <algorithm>
#include <chrono>

using namespace std::chrono;

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties) {
		FIF_PROFILE_FUNC();

		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(*this, windowProperties);

#ifndef __EMSCRIPTEN__
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
#endif

		FIF_LOG(glfwGetVersionString());
	}

	Application::~Application() {
		FIF_PROFILE_FUNC();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void Application::start() {
		// TODO: ImgGui module
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init("#version 300 es");
		ImGui_ImplGlfw_InitForOpenGL(mp_Window->getGlfwWindow(), true);

		FIF_PROFILE_FUNC();

#ifdef __EMSCRIPTEN__
		auto loop = []() {
			Application::getInstance().gameLoop();
		};

		emscripten_set_main_loop(loop, -1, true);
#else
		while(!mp_Window->getShouldClose())
			gameLoop();
#endif
	}

	void Application::gameLoop() {
		FIF_PROFILE_FUNC();

		const Clock::time_point now = Clock::now();
		const float dt = duration_cast<nanoseconds>(now - m_LastFrameTime).count() * 0.000000001f;
		m_LastFrameTime = now;

		m_LastFramePerformanceStats.fps = 1.0f / dt;
		m_LastFramePerformanceStats.frameTimeMs = dt * 1000.0f;

		update(dt);
		render();
	}

	void Application::update(float dt) {
		FIF_PROFILE_FUNC();
		fif::core::Profiler::clear();

		for(auto &mod : m_Modules)
			mod->update(dt);

		for(auto &layer : m_Layers)
			layer->update(dt);
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules)
			mod->render();

		for(auto &layer : m_Layers)
			layer->render();

		// TODO: ImGui as a separate module
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		for(auto &layer : m_Layers)
			layer->renderImGui();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mp_Window->endFrame();
	}

	void Application::onEvent(Event &event) {
		FIF_PROFILE_FUNC();

		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowResizeEvent>([&](WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.getSize().x, resizeEvent.getSize().y);
			return false;
		});
	}
}
