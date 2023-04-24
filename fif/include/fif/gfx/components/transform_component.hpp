#pragma once

#include "fif/core/ecs/component.hpp"
#include "glm/vec2.hpp"

namespace fif::gfx {
	class TransformComponent final : public core::Component {
	public:
		COMPONENT_NAME(TransformComponent)
		TransformComponent(const glm::vec2 &position = {0.0f, 0.0f}, float angle = 0.0f);

		glm::vec2 m_Position;
		float m_Angle;
	};
}// namespace fif::gfx
