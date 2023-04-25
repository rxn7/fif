#include "fif/gfx/components/transform_component.hpp"

namespace fif::gfx {
	TransformComponent::TransformComponent(const glm::vec2 &position, f32 angle) : m_Position(position), m_Angle(angle) {}
}// namespace fif::gfx
