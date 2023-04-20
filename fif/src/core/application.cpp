#include "fif/core/application.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/windowEvent.h"
#include "fif/core/opengl.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"
#include "fif/core/util/assertion.h"
#include "fif/core/util/clock.h"
#include "fif/core/util/rng.h"
#include "fif/core/util/timing.h"

#include <algorithm>
#include <chrono>

using namespace std::chrono;

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties) {
		FIF_PROFILE_FUNC();

		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		Rng::init();

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

		for (const auto &mod : m_Modules)
			mod->onStart(*this);

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop([]() { Application::getInstance().gameLoop(); }, -1, true);
#else
		while (!mp_Window->getShouldClose())
			gameLoop();
#endif
	}

	void Application::gameLoop() {
		FIF_PROFILE_FUNC();

		Time::update();

		m_PerformanceStats.fps = 1.0f / Time::getDeltaTime();
		m_PerformanceStats.frameTimeMs = Time::getDeltaTime() * 1000.0f;

		fif::core::Profiler::beginFrame();

		update();
		render();
	}

	void Application::update() {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules)
			mod->onUpdate();
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules)
			mod->onRender();

		for (auto &ent : m_Entities)
			ent.render();

		mp_Window->endFrame();
	}

	void Application::onEvent(Event &event) {
		FIF_PROFILE_FUNC();

		EventDispatcher::dispatch<WindowResizeEvent>(event, [&](WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.getSize().x, resizeEvent.getSize().y);
			return true;
		});

		for (auto &mod : m_Modules)
			mod->onEvent(event);

		for (auto &ent : m_Entities)
			ent.onEvent(event);
	}
} // namespace fif::core