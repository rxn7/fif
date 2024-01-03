#pragma once

namespace fif::core {
	class Entity final {
	public:
		Entity(Scene &scene, EntityID id);

		inline operator bool() {
			return m_ID != entt::null;
		}

		inline void delete_self() {
			m_Scene.delete_entity(m_ID);
			m_ID = entt::null;
		}

		template<typename T> bool has_component() const {
			return m_Scene.has_component<T>(m_ID);
		}

		template<typename T> T &get_component() const {
			return m_Scene.get_component<T>(m_ID);
		}

		template<typename T> T *try_get_component() const {
			return m_Scene.get_registry().try_get<T>(m_ID);
		}

		template<typename T, typename... Args> T &add_component(Args &&...args) {
			return m_Scene.add_component<T>(m_ID, std::forward<Args>(args)...);
		}

		template<typename T> void remove_component() {
			m_Scene.remove_component<T>(m_ID);
		}

		template<typename T, typename... Args> T &require_component(Args &&...args) {
			if(m_Scene.has_component<T>(m_ID))
				return m_Scene.get_component<T>(m_ID);

			return m_Scene.add_component<T>(m_ID, std::forward<Args>(args)...);
		}

	public:
		Scene &m_Scene;
		EntityID m_ID = entt::null;
	};
}// namespace fif::core
