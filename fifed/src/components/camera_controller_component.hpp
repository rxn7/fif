#pragma once

#include "fif/core/ecs/component.hpp"
#include "fif/core/event/event.hpp"
#include "fif/gfx/ortho_camera.hpp"

#include "glm/vec2.hpp"

class CameraControllerComponent final : public fif::core::Component {
public:
	CameraControllerComponent();
	COMPONENT_NAME(CameraControllerComponent);
	void on_event(fif::core::Event &event) override;
	void on_update() override;

public:
	float m_MaxZoom = 100.0f;
	float m_MinZoom = 0.005f;
	float m_ZoomLerpSpeed = 20.0f;

private:
	friend class EditorModule;
	glm::vec2 m_ViewportPosition;

	friend class EditorModule;
	glm::vec2 m_ViewportSize;

	glm::vec2 m_TargetPosition = {0, 0};
	float m_TargetZoom = 1.0f;

	bool m_IsZooming = false;

	fif::gfx::OrthoCamera &m_Camera;
};
