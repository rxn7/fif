#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/uuid_component.hpp"

namespace fif::core {
	Scene::~Scene() { Logger::debug("Scene::dtor"); }

	EntityID Scene::create_entity(const std::string &name) { return create_entity_with_uuid(UUID(), name); }

	EntityID Scene::create_entity_with_uuid(const UUID uuid, const std::string &name) {
		EntityID ent = m_Registry.create();
		add_component<UuidComponent>(ent).uuid = uuid;
		add_component<TagComponent>(ent, name);
		return ent;
	}

	EntityID Scene::duplicate_entity(const EntityID sourceID) {
		Entity sourceEntity(*this, sourceID);

		std::string name;
		if(TagComponent *tagComponent = sourceEntity.try_get_component<TagComponent>()) {
			// TODO: Change the name to be unique from the source (like in godot or unity)
			name = tagComponent->tag;
		} else {
			name = "Entity";
		}

		const EntityID targetEntityID = m_Registry.create();
		for(auto [id, storage] : m_Registry.storage()) {
			if(storage.contains(sourceID)) {
				storage.push(targetEntityID, storage.value(sourceID));
			}
		}

		Entity targetEntity(*this, targetEntityID);
		targetEntity.require_component<TagComponent>().tag = name;
		targetEntity.require_component<UuidComponent>().uuid = UUID();

		return targetEntityID;
	}
}// namespace fif::core
