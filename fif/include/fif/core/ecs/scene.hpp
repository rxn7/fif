#pragma once

#include "fif/core/ecs/entity.hpp"
#include <functional>

namespace fif::core {
	class Scene {
	public:
		virtual ~Scene();

		virtual void forEach(const std::function<void(Entity &ent)> &func);
		virtual void eraseDeletedEntities();

		virtual inline u64 getEntityCount() const { return m_Entities.size(); }

		virtual inline Entity *createEntity(const std::string &name) { return &m_Entities.emplace_back(name); }

	protected:
		std::vector<Entity> m_Entities;
	};
}// namespace fif::core
