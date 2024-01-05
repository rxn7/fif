#include "fif/core/ecs/entity.hpp"

namespace fif::core {
	Entity::Entity(core::Scene &scene, core::EntityID id) : m_Scene(scene), m_ID(id) {
	}
}// namespace fif::core
