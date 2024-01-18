#include "runtime_module.hpp"

#include <fif/core/event/event_dispatcher.hpp>
#include <fif/core/event/window_event.hpp>
#include <fif/core/project.hpp>
#include <fif/core/serialization/scene_serializer.hpp>

namespace fif_runtime {
	RuntimeModule::RuntimeModule() :
		m_FrameBuffer({0, 0}), m_StartCallback(std::bind(&RuntimeModule::on_start, this)), m_UpdateCallback(std::bind(&RuntimeModule::on_update, this)), m_EventCallback(std::bind(&RuntimeModule::on_event, this, std::placeholders::_1)) {
		FIF_MODULE_INIT();
		Application::get_instance().m_StartHook.hook(m_StartCallback);
		Application::get_instance().m_UpdateHook.hook(m_UpdateCallback);
		Application::get_instance().m_EventHook.hook(m_EventCallback);
	}

	RuntimeModule::~RuntimeModule() {
		mp_Application->m_StartHook.unhook(m_StartCallback);
		mp_Application->m_UpdateHook.unhook(m_UpdateCallback);
		mp_Application->m_EventHook.unhook(m_EventCallback);
	}

	void RuntimeModule::on_start() {
		FIF_ASSERT(std::filesystem::exists("./project.fifproj"), "project.fifproj file is missing! Did you export the project properly?");

		Project::load(".");

		FIF_ASSERT(std::filesystem::exists(Project::get_config().startingScenePath), "Starting scene doesn't exist!");

		mp_Application->get_window().set_title(Project::get_config().name);

		SceneSerializer serializer(Application::get_instance().get_scene());
		serializer.deserialize(Project::get_config().startingScenePath);
	}

	void RuntimeModule::on_update() {}

	void RuntimeModule::on_event(Event &event) {
		EventDispatcher::dispatch<WindowResizeEvent>(event, [&](WindowResizeEvent &resizeEvent) -> bool {
			GfxModule::get_instance().set_viewport(resizeEvent.get_size());
			return true;
		});
	}
}// namespace fif_runtime
