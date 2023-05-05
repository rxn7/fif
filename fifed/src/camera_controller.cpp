#include "camera_controller.hpp"

#include "event/mouse_event.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "gfx_module.hpp"

namespace fifed {
	void CameraController::update() {
		Renderer2D &renderer2D = GfxModule::get_instance()->get_renderer2D();
		OrthoCamera &cam = renderer2D.get_camera();

		if(isZooming) {
			const glm::vec2 mousePosition = InputModule::get_instance()->get_mouse_position();
			const glm::vec2 mouseWorldPositionBeforeZoom = cam.screen_to_world(mousePosition);

			cam.m_Zoom = glm::lerp(cam.m_Zoom, targetZoom, zoomLerpSpeed * Timing::get_delta_time());

			if(std::abs(cam.m_Zoom - targetZoom) <= 0.0001f) {
				cam.m_Zoom = targetZoom;
				isZooming = false;
			}

			cam.update_size();

			const glm::vec2 mouseWorldPositionAfterZoom = cam.screen_to_world(mousePosition);
			const glm::vec2 delta = mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;

			cam.m_Position += delta;
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

			targetZoom *= scrollEvent.get_value().y > 0 ? 0.9f : 1.1f;
			targetZoom = std::clamp(targetZoom, minZoom, maxZoom);

			isZooming = targetZoom != cam.m_Zoom;

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
