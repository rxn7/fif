#pragma once

#include "fif/core/uuid.hpp"

#include <entt/entity/registry.hpp>

namespace fif::core {
	typedef entt::entity EntityID;

	class Scene final {
	public:
		~Scene();

		EntityID create_entity(const char *name = "Entity");
		EntityID create_entity_with_uuid(UUID uuid, const char *name = "Entity");

		EntityID duplicate_entity(EntityID source);

		inline void for_each(const std::function<void(EntityID entityID)> &func) {
			for(EntityID entityID : m_Registry.view<EntityID>()) {
				func(entityID);
			}
		}

		inline void delete_entity(EntityID entity) { m_Registry.destroy(entity); }

		inline void clear() { m_Registry.clear(); }

		inline u64 get_entity_count() const { return m_Registry.storage<EntityID>()->free_list(); }

		inline entt::registry &get_registry() { return m_Registry; }

		template<typename T> inline bool has_component(EntityID entity) { return m_Registry.any_of<T>(entity); }

		template<typename T> inline T &get_component(EntityID entity) {
			FIF_ASSERT(has_component<T>(entity), "This entity doesn't have a component of this type (%s)!", typeid(T).name());
			return m_Registry.get<T>(entity);
		}

		template<typename T, typename... Args> inline T &add_component(EntityID entity, Args &&...args) {
			FIF_ASSERT(!has_component<T>(entity), "This entity already has component of this type (%s)!", typeid(T).name());
			return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
		}

		template<typename T> inline void remove_component(EntityID entity) {
			FIF_ASSERT(has_component<T>(entity), "This entity doesn't have a component of this type (%s)!", typeid(T).name());
			m_Registry.erase<T>(entity);
		}

	protected:
		entt::registry m_Registry;
	};
}// namespace fif::core
