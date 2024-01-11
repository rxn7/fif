#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/uuid_component.hpp"

namespace fif::core {
	Scene::~Scene() { Logger::debug("Scene::dtor"); }

	EntityID Scene::create_entity(const char *name) { return create_entity_with_uuid(UUID(), name); }

	EntityID Scene::create_entity_with_uuid(UUID uuid, const char *name) {
		EntityID ent = m_Registry.create();
		add_component<UuidComponent>(ent).uuid = uuid;
		add_component<TagComponent>(ent, name);
		return ent;
	}
}// namespace fif::core
