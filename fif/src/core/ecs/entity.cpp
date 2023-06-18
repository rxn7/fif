#include "fif/core/ecs/entity.hpp"

namespace fif::core {
	Entity::Entity(core::Scene *scene, core::EntityID id) : mp_Scene(scene), m_ID(id) {}
}// namespace fif::core
