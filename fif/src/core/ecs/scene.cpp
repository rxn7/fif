#include "fif/core/ecs/scene.hpp"

namespace fif::core {
	Scene::~Scene() {}

	void Scene::forEach(const std::function<void(Entity &ent)> &func) {
		for(Entity &ent : m_Entities) {
			func(ent);
		}
	}

	void Scene::eraseDeletedEntities() {
		auto entIter = m_Entities.begin();
		while(entIter != m_Entities.end()) {
			if(entIter->isDeleteQueued()) {
				entIter = m_Entities.erase(entIter);
				continue;
			}

			(entIter++)->update();
		}
	}
}// namespace fif::core
