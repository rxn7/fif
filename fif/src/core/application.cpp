#include "fif/core/application.hpp"
#include "fif/core/event/eventDispatcher.hpp"
#include "fif/core/event/windowEvent.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/core/util/timing.hpp"

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

		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	}

	Application::~Application() {
		FIF_PROFILE_FUNC();
	}

	void Application::start() {
		FIF_PROFILE_FUNC();

		for (const auto &mod : m_Modules) {
			mod->onStart(*this);
		}

		while (!mp_Window->getShouldClose()) {
			gameLoop();
		}
	}

	void Application::gameLoop() {
		FIF_PROFILE_FUNC();

		Timing::update();

		m_PerformanceStats.fps = 1.0F / Timing::getDeltaTime();
		m_PerformanceStats.frameTimeMs = Timing::getDeltaTime() * 1000.0F;

		fif::core::Profiler::beginFrame();

		update();
		render();
	}

	void Application::update() {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules) {
			mod->onUpdate();
		}

		auto entIter = m_Entities.begin();
		while (entIter != m_Entities.end()) {
			if (entIter->isDeleteQueued()) {
				entIter = m_Entities.erase(entIter);
				continue;
			}

			(entIter++)->update();
		}
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules) {
			mod->onRender();
		}

		for (auto &ent : m_Entities) {
			ent.render();
		}

		mp_Window->endFrame();
	}

	void Application::onEvent(Event &event) {
		FIF_PROFILE_FUNC();

		// TODO: Should gfx module handle this or core?
		EventDispatcher::dispatch<WindowResizeEvent>(event, [&](WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.getSize().x, resizeEvent.getSize().y);
			return true;
		});

		for (auto &mod : m_Modules) {
			mod->onEvent(event);
		}

		for (auto &ent : m_Entities) {
			ent.onEvent(event);
		}
	}
} // namespace fif::core
