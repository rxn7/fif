#include "camera_controller_component.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/profiler.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/input/input_module.hpp"
#include "fif/core/util/timing.hpp"

#include "glm/gtx/compatibility.hpp"

CameraControllerComponent::CameraControllerComponent(fif::gfx::OrthoCamera &camera) : m_Camera(camera) {}

void CameraControllerComponent::on_update() {
	f32 dt = fif::core::Timing::get_delta_time();
	m_Camera.m_Position = glm::lerp(m_Camera.m_Position, m_TargetPosition, 10 * dt);
	m_Camera.m_Zoom = glm::lerp(m_Camera.m_Zoom, m_TargetZoom, 10 * dt);
}

void CameraControllerComponent::on_event(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if(scrollEvent.is_hanlded() || scrollEvent.get_value().y == 0) {
			return false;
		}

		const glm::vec2 mousePosition = fif::input::InputModule::get_instance()->get_mouse_position();
		const glm::vec2 mouseWorldPositionBeforeZoom = m_Camera.screen_to_world(mousePosition);

		m_TargetZoom = m_Camera.m_Zoom * scrollEvent.get_value().y > 0 ? 0.9F : 1.1F;
		m_TargetZoom = std::clamp(m_TargetZoom, 0.05F, 100.0F);

		const glm::vec2 mouseWorldPositionAfterZoom = m_Camera.screen_to_world(mousePosition);
		const glm::vec2 delta = mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;

		m_TargetPosition += m_Camera.m_Position + delta;

		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if(movedEvent.is_hanlded() || !fif::input::InputModule::get_instance()->is_button_held(GLFW_MOUSE_BUTTON_RIGHT)) {
			return false;
		}

		const glm::vec2 mouseWorldPosition = m_Camera.screen_to_world(movedEvent.get_position());
		const glm::vec2 lastMouseWorldPosition = m_Camera.screen_to_world(fif::input::InputModule::get_instance()->get_last_mouse_position());
		const glm::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

		m_TargetPosition = m_Camera.m_Position - delta;

		return true;
	});
}
