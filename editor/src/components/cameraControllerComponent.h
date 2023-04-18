#pragma once

#include "fif/core/ecs/component.h"
#include "fif/core/event/event.h"
#include "fif/gfx/orthoCamera.h"
#include "glm/vec2.hpp"

class CameraControllerComponent final : public fif::core::Component {
  public:
	CameraControllerComponent(fif::gfx::OrthoCamera &camera);
	COMPONENT_NAME(CameraControllerComponent);
	void onEvent(fif::core::Event &event) override;

  private:
	fif::gfx::OrthoCamera &m_Camera;
};
