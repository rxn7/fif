#include "cameraControllerComponent.h"

#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/profiler.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
#include "fif/input/inputModule.h"
#include "glm/gtx/compatibility.hpp"

CameraControllerComponent::CameraControllerComponent(fif::gfx::OrthoCamera &camera) : m_Camera(camera) {}

void CameraControllerComponent::onEvent(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if (scrollEvent.isHanlded() || scrollEvent.getValue().y == 0)
			return false;

		glm::vec2 mousePosition = fif::input::InputModule::getInstance()->getMousePosition();

		glm::vec2 mouseWorldPositionBeforeZoom = m_Camera.screenToWorld(mousePosition);

		m_Camera.m_Zoom *= scrollEvent.getValue().y > 0 ? 0.9f : 1.1f;
		m_Camera.update();

		glm::vec2 mouseWorldPositionAfterZoom = m_Camera.screenToWorld(mousePosition);

		m_Camera.m_Position += mouseWorldPositionBeforeZoom - mouseWorldPositionAfterZoom;

		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if (!fif::input::InputModule::getInstance()->isButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
			return false;

		glm::vec2 mouseWorldPosition = m_Camera.screenToWorld(movedEvent.getPosition());
		glm::vec2 lastMouseWorldPosition = m_Camera.screenToWorld(fif::input::InputModule::getInstance()->getLastMousePosition());
		glm::vec2 delta = mouseWorldPosition - lastMouseWorldPosition;

		m_Camera.m_Position -= delta;

		return true;
	});
}
