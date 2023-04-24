#include "fif/core/application.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/core/util/timing.hpp"

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties, bool createDefaultScene) {
		FIF_PROFILE_FUNC();

		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(*this, windowProperties);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		if(createDefaultScene) {
			mp_Scene = std::make_shared<Scene>();
		}

		Rng::init();
	}

	Application::~Application() {
		FIF_PROFILE_FUNC();
	}

	void Application::start() {
		FIF_PROFILE_FUNC();

		for(const auto &mod : m_Modules) {
			mod->on_start(*this);
		}

		while(!mp_Window->get_should_close()) {
			game_loop();
		}
	}

	void Application::game_loop() {
		FIF_PROFILE_FUNC();

		Timing::update();

		m_PerformanceStats.fps = 1.0F / Timing::get_delta_time();
		m_PerformanceStats.frameTimeMs = Timing::get_delta_time() * 1000.0F;

		fif::core::Profiler::begin_frame();

		update();
		render();
	}

	void Application::update() {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules) {
			mod->on_update();
		}

		if(mp_Scene) {
			mp_Scene->for_each([&](Entity &ent) { ent.update(); });
		}
	}

	void Application::render() {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules) {
			mod->on_render();
		}

		if(mp_Scene) {
			mp_Scene->for_each([&](Entity &ent) { ent.render(); });
		}

		mp_Window->end_frame();
	}

	void Application::on_event(Event &event) {
		FIF_PROFILE_FUNC();

		for(auto &mod : m_Modules) {
			mod->on_event(event);
		}

		if(mp_Scene) {
			mp_Scene->for_each([&](Entity &ent) { ent.on_event(event); });
		}
	}
}// namespace fif::core
