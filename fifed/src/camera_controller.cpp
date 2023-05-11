#include "camera_controller.hpp"

#include "event/mouse_event.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	f32 CameraController::s_ZoomLerpDuration = 0.5f;
	f32 CameraController::s_MaxZoom = 1000.0f;
	f32 CameraController::s_MinZoom = 0.010f;
	glm::vec2 CameraController::s_ViewportSize;
	glm::vec2 CameraController::s_ViewportPosition;

	static float s_TargetZoom = 1.0f;
	static float s_StartZoom = 1.0f;
	static float s_SnapTolerance;
	static float s_ZoomTimer = 0.0f;
	static bool s_IsZooming = false;

	void CameraController::update() {
		Renderer2D &renderer2D = GfxModule::get_instance()->get_renderer2D();
		OrthoCamera &cam = renderer2D.get_camera();

		if(s_IsZooming) {
			Logger::info("zooming");

			const glm::vec2 mousePosition = InputModule::get_instance()->get_mouse_position();
			const glm::vec2 mouseWorldPositionBeforeZoom = cam.screen_to_world(mousePosition);

			if(s_ZoomLerpDuration > 0) {
				const f32 zoomLerpPercentage = s_ZoomTimer / s_ZoomLerpDuration;

				// Ease out algortihm
				const f32 smoothZoomLerpPercentage = glm::sin(zoomLerpPercentage * glm::pi<f32>() * 0.5f);

				if(zoomLerpPercentage < 1.0f) {
					cam.m_Zoom = glm::lerp(s_StartZoom, s_TargetZoom, smoothZoomLerpPercentage);

					s_IsZooming = true;
					s_ZoomTimer += Timing::get_delta_time();
				} else {
					cam.m_Zoom = s_TargetZoom;

					s_IsZooming = false;
				}
			} else {
				cam.m_Zoom = s_TargetZoom;
				s_IsZooming = false;
			}

			cam.update_size();

			const glm::vec2 mouseWorldPositionAfterZoom = cam.screen_to_world(mousePosition);
			const glm::vec2 mousePostionDelta = mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;
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

		const glm::vec2 mousePosition = gfx->get_point_relative_to_viewport(input->get_mouse_position());
		if(mousePosition.x < 0 || mousePosition.y < 0 || mousePosition.x > gfx->get_viewport_size().x || mousePosition.y > gfx->get_viewport_size().y)
			return;

		EventDispatcher::dispatch<MouseScrolledEvent>(event, [&](MouseScrolledEvent &scrollEvent) {
			if(scrollEvent.get_value().y == 0)
				return false;

			s_StartZoom = cam.m_Zoom;
			s_TargetZoom *= scrollEvent.get_value().y > 0 ? 0.9f : 1.1f;
			s_TargetZoom = std::clamp(s_TargetZoom, s_MinZoom, s_MaxZoom);
			s_SnapTolerance = std::log10(1.0f + s_TargetZoom) * 0.001f;

			s_ZoomTimer = 0.0f;
			s_IsZooming = s_TargetZoom != cam.m_Zoom;

			return true;
		});

		EventDispatcher::dispatch<MouseMovedEvent>(event, [&](MouseMovedEvent &movedEvent) {
			if(!input->is_button_held(GLFW_MOUSE_BUTTON_RIGHT))
				return false;

			const glm::vec2 mouseWorldPosition = cam.screen_to_world(movedEvent.get_position());
			const glm::vec2 lastMouseWorldPosition = cam.screen_to_world(input->get_last_mouse_position());
			const glm::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

			cam.m_Position -= delta;

			return true;
		});
	}
}// namespace fifed
