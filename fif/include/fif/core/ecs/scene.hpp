#pragma once

#include "fif/core/ecs/entity.hpp"
#include <functional>

namespace fif::core {
	class Scene {
	public:
		virtual ~Scene();

		virtual void for_each(const std::function<void(Entity &ent)> &func);
		virtual void erase_deleted_entities();

		virtual inline u64 get_entity_count() const { return m_Entities.size(); }

		virtual inline Entity *create_entity(const std::string &name) { return &m_Entities.emplace_back(name); }

	protected:
		std::vector<Entity> m_Entities;
	};
}// namespace fif::core
