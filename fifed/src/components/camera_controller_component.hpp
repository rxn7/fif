#pragma once

#include "fif/core/ecs/component.hpp"
#include "fif/core/event/event.hpp"
#include "fif/gfx/ortho_camera.hpp"

#include "glm/vec2.hpp"

class CameraControllerComponent final : public fif::core::Component {
public:
	CameraControllerComponent(fif::gfx::OrthoCamera &camera);
	COMPONENT_NAME(CameraControllerComponent);
	void on_event(fif::core::Event &event) override;
	void on_update() override;

private:
	float m_TargetZoom = 1.0f;
	glm::vec2 m_TargetPosition = {0,0};
	fif::gfx::OrthoCamera &m_Camera;
};
