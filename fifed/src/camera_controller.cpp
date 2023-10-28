#include "camera_controller.hpp"

#include "event/mouse_event.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	void CameraController::update() {
		Renderer2D &renderer2D = GfxModule::get_instance()->get_renderer2D();
		OrthoCamera &cam = renderer2D.get_camera();

		if(m_IsZooming) {
			const fif::vec2 mouseWorldPositionBeforeZoom = cam.screen_to_world(m_StartMousePositionLocal);

			if(m_ZoomLerpDuration > 0) {
				const f32 zoomLerpPercentage = m_ZoomTimer / m_ZoomLerpDuration;
				const f32 smoothZoomLerpPercentage = glm::sin(zoomLerpPercentage * glm::pi<f32>() * 0.5f);

				if(zoomLerpPercentage < 1.0f) {
					cam.m_Zoom = glm::lerp(m_StartZoom, m_TargetZoom, smoothZoomLerpPercentage);

					m_IsZooming = true;
					m_ZoomTimer += Timing::get_delta_time();
				} else {
					cam.m_Zoom = m_TargetZoom;

					m_IsZooming = false;
				}
			} else {
				cam.m_Zoom = m_TargetZoom;
				m_IsZooming = false;
			}

			cam.update_size();

			const fif::vec2 mouseWorldPositionAfterZoom = cam.screen_to_world(m_StartMousePositionLocal);
			const fif::vec2 mousePostionDelta = mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;
			cam.m_Position += mousePostionDelta;
		}
	}

	void CameraController::on_event(Event &event, bool viewportHovered) {
		if(event.m_Handled || !viewportHovered)
			return;

		GfxModule *gfx = GfxModule::get_instance();
		InputModule *input = InputModule::get_instance();
		Renderer2D &renderer2D = gfx->get_renderer2D();
		OrthoCamera &cam = renderer2D.get_camera();

		const fif::vec2 mousePosition = input->get_mouse_position();
		const fif::vec2 mousePositionRelativeToViewport = gfx->get_point_relative_to_viewport(mousePosition);
		if(mousePositionRelativeToViewport.x < 0 || mousePositionRelativeToViewport.y < 0 ||
		   mousePositionRelativeToViewport.x > gfx->get_viewport_size().x || mousePositionRelativeToViewport.y > gfx->get_viewport_size().y)
			return;

		EventDispatcher::dispatch<MouseScrolledEvent>(event, [&](MouseScrolledEvent &scrollEvent) {
			if(scrollEvent.get_value().y == 0)
				return false;

			m_StartZoom = cam.m_Zoom;
			m_TargetZoom *= scrollEvent.get_value().y > 0 ? 0.9f : 1.1f;
			m_TargetZoom = std::clamp(m_TargetZoom, m_MinZoom, m_MaxZoom);
			m_StartMousePositionLocal = mousePosition;

			m_ZoomTimer = 0.0f;
			m_IsZooming = m_TargetZoom != cam.m_Zoom;

			return true;
		});

		EventDispatcher::dispatch<MouseMovedEvent>(event, [&](MouseMovedEvent &movedEvent) {
			if(!input->is_button_held(GLFW_MOUSE_BUTTON_RIGHT))
				return false;

			const fif::vec2 mouseWorldPosition = cam.screen_to_world(movedEvent.get_position());
			const fif::vec2 lastMouseWorldPosition = cam.screen_to_world(input->get_last_mouse_position());
			const fif::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

			cam.m_Position -= delta;

			return true;
		});
	}
}// namespace fifed
