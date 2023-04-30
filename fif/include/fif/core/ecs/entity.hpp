#pragma once

#include "ecs/uuid.hpp"
#include "fif/core/ecs/component.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/profiler.hpp"

#include <memory>
#include <vector>

namespace fif::core {
	class Entity final {
	public:
		Entity();

		template<class T, class... Args> T *add_component(Args &&...args) {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			std::unique_ptr<Component> &comp = m_Components.emplace_back(new T(args...));
			comp->mp_Entity = this;
			comp->on_create();

			return static_cast<T *>(comp.get());
		}

		template<class T> T *get_component() {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			for(std::unique_ptr<Component> &comp : m_Components) {
				T *c = dynamic_cast<T *>(comp.get());
				if(c != nullptr)
					return c;
			}

			return nullptr;
		}

		inline void queue_delete() { m_DeleteQueued = true; }

		inline bool is_delete_queued() const { return m_DeleteQueued; }

		inline const std::vector<std::unique_ptr<Component>> &get_components() const { return m_Components; }

		inline UUID get_uuid() const { return m_UUID; }

		void on_update();
		void on_render();
		void on_event(Event &event);

	private:
		bool m_DeleteQueued = false;
		UUID m_UUID;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}// namespace fif::core
