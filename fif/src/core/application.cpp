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

	Application::Application(const WindowProperties &windowProperties, bool createDefaultScene) {
		FIF_PROFILE_FUNC();

		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(*this, windowProperties);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		if (createDefaultScene) {
			mp_Scene = std::make_shared<Scene>();
		}

		Rng::init();
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

		if (mp_Scene) {
			mp_Scene->forEach([&](Entity &ent) { ent.update(); });
		}
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules) {
			mod->onRender();
		}

		if (mp_Scene) {
			mp_Scene->forEach([&](Entity &ent) { ent.render(); });
		}

		mp_Window->endFrame();
	}

	void Application::onEvent(Event &event) {
		FIF_PROFILE_FUNC();

		for (auto &mod : m_Modules) {
			mod->onEvent(event);
		}

		if (mp_Scene) {
			mp_Scene->forEach([&](Entity &ent) { ent.onEvent(event); });
		}
	}
} // namespace fif::core
