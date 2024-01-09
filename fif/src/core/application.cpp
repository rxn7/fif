#include "./serialization/core_entity_serializer.hpp"

#include "fif/core/application.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/core/project.hpp"
#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/core/util/logger.hpp"

namespace fif::core {
	Application::Application(const ApplicationProperties &appProperties) {
		FIF_ASSERT(sp_Instance == nullptr, "Only 1 instance of fif::core::Application can exist!");
		sp_Instance = this;

		mp_Window = std::make_unique<Window>(*this, appProperties.windowProps);
		gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		Rng::init();
		Timing::init();

		if(appProperties.createDefaultScene)
			mp_Scene = std::make_unique<Scene>();
	}

	Application::~Application() {}

	void Application::start() {
		m_Status.running = true;

		setup_modules();

		SceneSerializer::add_serializer<CoreEntitySerializer>();
		m_StartHook.invoke();

		while(m_Status.running)
			game_loop();

		Project::get_active().reset();
		mp_Scene.reset();

		for(std::unique_ptr<Module> &module : m_Modules) {
			Logger::debug("Destroying module '%s'", module->get_name().data());
			module.reset();
		}
	}

	void Application::game_loop() {
		Timing::update();

		m_PerformanceStats.timer += Timing::get_delta_time();
		m_PerformanceStats.frameCount++;
		if(m_PerformanceStats.timer >= 1.0f) {
			m_PerformanceStats.fps = m_PerformanceStats.frameCount;
			m_PerformanceStats.timer = 0.0f;
			m_PerformanceStats.frameCount = 0u;
		}
		m_PerformanceStats.frameTimeMs = Timing::get_delta_time() * 1000.0f;

		update();

		// update() could change the status
		if(!m_Status.running)
			return;

		m_EndFrameHook.invoke();

		mp_Window->end_frame();
	}

	void Application::update() {
		m_PreUpdateHook.invoke();
		m_UpdateHook.invoke();

		for(auto &updateSystem : m_UpdateSystems)
			updateSystem(m_Status, mp_Scene->get_registry(), Timing::get_delta_time());
	}

	void Application::on_event(Event &event) {
		EventDispatcher::dispatch<WindowCloseEvent>(event, [&](WindowCloseEvent &) {
			Logger::info("Quitting...");
			m_Status.running = false;
			return true;
		});

		m_EventHook.invoke(event);

		for(auto &eventSystem : m_EventSystems)
			eventSystem(m_Status, mp_Scene->get_registry(), event);
	}
}// namespace fif::core
