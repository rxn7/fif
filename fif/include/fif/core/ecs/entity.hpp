#pragma once

#include "fif/core/ecs/component.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/profiler.hpp"

#include <memory>
#include <vector>

namespace fif::core {
	class Entity final {
	public:
		Entity(const std::string &name);

		template<class T, class... Args> T *addComponent(Args &&...args) {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			std::unique_ptr<Component> &comp = m_Components.emplace_back(new T(args...));
			comp->mp_Entity = this;
			comp->onCreate();

			return static_cast<T *>(comp.get());
		}

		template<class T> T *getComponent() {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			for(std::unique_ptr<Component> &comp : m_Components) {
				T *c = dynamic_cast<T *>(comp.get());
				if(c != nullptr)
					return c;
			}

			return nullptr;
		}

		inline const std::vector<std::unique_ptr<Component>> &getComponents() const { return m_Components; }

		inline bool isDeleteQueued() const { return m_DeleteQueued; }

		inline const std::string &getName() const { return m_Name; }

		inline void queueDelete() { m_DeleteQueued = true; }

		void update();
		void render();
		void onEvent(Event &event);

	private:
		bool m_DeleteQueued = false;
		std::string m_Name;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}// namespace fif::core
