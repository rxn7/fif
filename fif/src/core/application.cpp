#include "fif/core/application.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"

namespace fif::core {
	Application *fif::core::Application::s_Instance = nullptr;

	Application::Application(const ApplicationProperties &appProperties) {
		FIF_ASSERT(s_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		s_Instance = this;

		mp_Window = std::make_unique<Window>(*this, appProperties.windowProps);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		Rng::init();

		if(appProperties.createDefaultScene)
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

		for(auto &updateSystem : m_UpdateSystems)
			updateSystem(m_Status, mp_Scene->get_registry(), Timing::get_delta_time());
	}

	void Application::render() {
		for(auto &mod : m_Modules)
			mod->on_render();

		for(auto &renderSystem : m_RenderSystems)
			renderSystem(m_Status, mp_Scene->get_registry());

		mp_Window->end_frame();
	}

	void Application::on_event(Event &event) {
		for(auto &mod : m_Modules)
			mod->on_event(event);

		for(auto &eventSystem : m_EventSystems)
			eventSystem(m_Status, mp_Scene->get_registry(), event);
	}

	void Application::pause(bool paused) {
		m_Status.paused = paused;
	}
}// namespace fif::core
