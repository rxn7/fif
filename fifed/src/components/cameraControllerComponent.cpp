#include "cameraControllerComponent.hpp"

#include "fif/core/event/eventDispatcher.hpp"
#include "fif/core/event/mouseEvent.hpp"
#include "fif/core/profiler.hpp"
#include "fif/gfx/orthoCamera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/input/inputModule.hpp"

#include "glm/gtx/compatibility.hpp"

CameraControllerComponent::CameraControllerComponent(fif::gfx::OrthoCamera &camera) : m_Camera(camera) {}

void CameraControllerComponent::onEvent(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if(scrollEvent.isHanlded() || scrollEvent.getValue().y == 0) {
			return false;
		}

		const glm::vec2 mousePosition = fif::input::InputModule::getInstance()->getMousePosition();
		const glm::vec2 mouseWorldPositionBeforeZoom = m_Camera.screenToWorld(mousePosition);

		m_Camera.m_Zoom *= scrollEvent.getValue().y > 0 ? 0.9F : 1.1F;
		m_Camera.m_Zoom = std::clamp(m_Camera.m_Zoom, 0.05F, 100.0F);
		m_Camera.updateSize();

		const glm::vec2 mouseWorldPositionAfterZoom = m_Camera.screenToWorld(mousePosition);

		m_Camera.m_Position += mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;

		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if(movedEvent.isHanlded() || !fif::input::InputModule::getInstance()->isButtonHeld(GLFW_MOUSE_BUTTON_RIGHT)) {
			return false;
		}

		const glm::vec2 mouseWorldPosition = m_Camera.screenToWorld(movedEvent.getPosition());
		const glm::vec2 lastMouseWorldPosition = m_Camera.screenToWorld(fif::input::InputModule::getInstance()->getLastMousePosition());
		const glm::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

		m_Camera.m_Position -= delta;

		return true;
	});
}
