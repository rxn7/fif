#include "fif/core/application.h"
#include "fif/core/assertion.h"
#include "fif/core/clock.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/windowEvent.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"

#include "fif/core/opengl.h"

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
	}

	Application::~Application() {
		FIF_PROFILE_FUNC();
	}

	void Application::start() {
		FIF_PROFILE_FUNC();

		for(const auto &mod : m_Modules)
			mod->onStart(*this);

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop([](){ Application::getInstance().gameLoop(); }, -1, true);
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

		fif::core::Profiler::beginFrame();

		update(dt);
		render();
	}

	void Application::update(float dt) {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules)
			mod->onUpdate(dt);
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules)
			mod->onRender();

		mp_Window->endFrame();
	}

	void Application::onEvent(Event &event) {
		FIF_PROFILE_FUNC();

		EventDispatcher::dispatch<WindowResizeEvent>(event, [&](WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.getSize().x, resizeEvent.getSize().y);
			return true;
		});

		for(auto &mod : m_Modules)
			mod->onEvent(event);

		for(auto &ent : m_Entities)
			ent.onEvent(event);
	}
}
