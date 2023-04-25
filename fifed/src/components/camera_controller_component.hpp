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
};
