#pragma once

#include "fif/core/ecs/component.h"
#include "fif/core/event/event.h"

#include "glm/vec2.hpp"

class CameraControllerComponent final : public fif::core::Component {
public:
	COMPONENT_NAME(CameraControllerComponent);
	void onEvent(fif::core::Event &event) override;
};
