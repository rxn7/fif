#include "cameraControllerComponent.h"

#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/profiler.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
#include "fif/input/inputModule.h"

void CameraControllerComponent::onEvent(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if(scrollEvent.isHanlded() || scrollEvent.getValue().y == 0)
			return false;

		fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::getCamera();
		cam.m_Zoom = std::clamp(cam.m_Zoom - scrollEvent.getValue().y * 0.1f, 0.1f, 10.0f);
		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if(!fif::input::InputModule::getInstance()->isButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
			return false;

		fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::getCamera();
		cam.m_Position.x -= movedEvent.getDelta().x * cam.m_Zoom;
		cam.m_Position.y += movedEvent.getDelta().y * cam.m_Zoom;
		return true;
	});
}
