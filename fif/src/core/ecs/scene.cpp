#include "fif/core/ecs/scene.hpp"

namespace fif::core {
	Scene::~Scene() {}

	void Scene::for_each(const std::function<void(Entity &ent)> &func) {
		for(Entity &ent : m_Entities)
			func(ent);
	}

	void Scene::erase_deleted_entities() {
		auto entIter = m_Entities.begin();
		while(entIter != m_Entities.end()) {
			if(entIter->is_delete_queued()) {
				entIter = m_Entities.erase(entIter);
				continue;
			}

			(entIter++)->on_update();
		}
	}
}// namespace fif::core
