#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/components/tag_component.hpp"

namespace fif::core {
	EntityID Scene::create_entity(const std::string &name) {
		EntityID ent = m_Registry.create();
		add_component<TagComponent>(ent, name);
		return ent;
	}

	EntityID Scene::duplicate_entity([[maybe_unused]] EntityID source) {
		FIF_LOG_ERROR("This function is not implemented");
		return entt::null;

		// FIXME: This doesnt work, creates multiple entities
		/*
		EntityID ent = m_Registry.create();

		for(auto [id, storage] : m_Registry.storage())
			if(storage.contains(source))
				storage.push(ent, storage.value(source));

		return ent;
		*/
	}
}// namespace fif::core
