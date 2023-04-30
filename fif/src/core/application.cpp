#include "fif/core/application.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/util/rng.hpp"
#include "fif/core/util/timing.hpp"

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const WindowProperties &windowProperties, bool createDefaultScene) {
		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(*this, windowProperties);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		Rng::init();

		if(createDefaultScene)
			mp_Scene = std::make_unique<Scene>();
	}

	Application::~Application() {}

	void Application::start() {
		for(const auto &mod : m_Modules)
			mod->on_start(*this);

		while(!mp_Window->get_should_close())
			game_loop();
	}

	void Application::game_loop() {
		Timing::update();

		m_PerformanceStats.fps = 1.0F / Timing::get_delta_time();
		m_PerformanceStats.frameTimeMs = Timing::get_delta_time() * 1000.0F;

		update();
		render();
	}

	void Application::update() {
		for(auto &mod : m_Modules)
			mod->on_update();

		for(auto &system : m_UpdateSystems)
			system(mp_Scene->get_registry(), Timing::get_delta_time());
	}

	void Application::render() {
		for(auto &mod : m_Modules)
			mod->on_render();

		for(auto &system : m_RenderSystems)
			system(mp_Scene->get_registry());

		mp_Window->end_frame();
	}

	void Application::on_event(Event &event) {
		for(auto &mod : m_Modules)
			mod->on_event(event);

		for(auto &system : m_EventSystems)
			system(mp_Scene->get_registry(), event);
	}
}// namespace fif::core
