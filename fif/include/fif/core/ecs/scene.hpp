#pragma once

#include "fif/core/types.hpp"
#include "fif/core/util/assertion.hpp"

#include "entt/entt.hpp"
#include <functional>

namespace fif::core {
	typedef entt::entity EntityID;

	class Scene {
	public:
		inline void for_each(const std::function<void(EntityID &ent)> &func) { m_Registry.each(func); }

		inline EntityID create_entity() { return m_Registry.create(); }

		inline void delete_entity(EntityID entity) { m_Registry.destroy(entity); }

		template<typename T> inline bool has_component(EntityID entity) { return m_Registry.any_of<T>(entity); }

		template<typename T> T &get_component(EntityID entity) {
			FIF_ASSERT(has_component<T>(entity), "This entity doesn't have a component of this type (" << typeid(T).name() << ")!");
			return m_Registry.get<T>(entity);
		}

		template<typename T, typename... Args> T &add_component(EntityID entity, Args &&...args) {
			FIF_ASSERT(!has_component<T>(entity), "This entity already has a component of this type (" << typeid(T).name() << ")!");
			return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
		}

		template<typename T> T &remove_component(EntityID entity) {
			FIF_ASSERT(has_component<T>(entity), "This entity doesn't have a component of this type (" << typeid(T).name() << ")!");
			m_Registry.remove<T>(entity);
		}

		inline u64 get_entity_count() const { return m_Registry.storage<EntityID>().in_use(); }
		inline entt::registry &get_registry() { return m_Registry; }

	protected:
		entt::registry m_Registry;
	};
}// namespace fif::core
