#pragma once

namespace fif::core {
	class Entity final {
	public:
		Entity() {}
		Entity(core::Scene *scene, core::EntityID id);

		template<typename T> bool has_component() { return mp_Scene->has_component<T>(m_ID); }
		template<typename T> T &get_component() { return mp_Scene->get_component<T>(m_ID); }
		template<typename T, typename... Args> T &add_component(Args &&...args) { return mp_Scene->add_component<T>(m_ID, std::forward(args)...); }
		template<typename T, typename... Args> void remove_component() { mp_Scene->remove_component<T>(m_ID); }

		const core::Scene &get_scene() const { return *mp_Scene; }
		core::EntityID get_id() const { return m_ID; }

	public:
		core::Scene *mp_Scene;
		core::EntityID m_ID;
	};
}// namespace fif::core
