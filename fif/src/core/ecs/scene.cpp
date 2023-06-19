#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/components/tag_component.hpp"

namespace fif::core {
	EntityID Scene::create_entity(const char *name) {
		EntityID ent = m_Registry.create();
		add_component<TagComponent>(ent, name);
		return ent;
	}
}// namespace fif::core
