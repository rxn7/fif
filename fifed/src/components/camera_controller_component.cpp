#include "camera_controller_component.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/profiler.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/input/input_module.hpp"
#include "fif/core/util/timing.hpp"

#include "glm/gtx/compatibility.hpp"

CameraControllerComponent::CameraControllerComponent() : m_Camera(fif::gfx::Renderer2D::get_camera()) {}

void CameraControllerComponent::on_update() {
	f32 dt = fif::core::Timing::get_delta_time();

	const glm::vec2 mousePosition = fif::input::InputModule::get_instance()->get_mouse_position();
	const glm::vec2 mouseWorldPositionBeforeZoom = m_Camera.screen_to_world(mousePosition);

	m_Camera.m_Zoom = glm::lerp(m_Camera.m_Zoom, m_TargetZoom, 20 * dt);
	m_Camera.update_size();

	const glm::vec2 mouseWorldPositionAfterZoom = m_Camera.screen_to_world(mousePosition);
	const glm::vec2 delta = mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;

	m_Camera.m_Position += delta;
}

void CameraControllerComponent::on_event(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if(scrollEvent.is_hanlded() || scrollEvent.get_value().y == 0) {
			return false;
		}

		m_TargetZoom *= scrollEvent.get_value().y > 0 ? 0.9F : 1.1F;
		m_TargetZoom = std::clamp(m_TargetZoom, 0.05F, 100.0F);

		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if(movedEvent.is_hanlded() || !fif::input::InputModule::get_instance()->is_button_held(GLFW_MOUSE_BUTTON_RIGHT)) {
			return false;
		}

		const glm::vec2 mouseWorldPosition = m_Camera.screen_to_world(movedEvent.get_position());
		const glm::vec2 lastMouseWorldPosition = m_Camera.screen_to_world(fif::input::InputModule::get_instance()->get_last_mouse_position());
		const glm::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

		m_Camera.m_Position -= delta;

		return true;
	});
}
