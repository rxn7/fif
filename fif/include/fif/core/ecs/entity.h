#pragma once

#include "fif/core/ecs/component.h"
#include "fif/core/event/event.h"
#include "fif/core/profiler.h"

#include <memory>
#include <vector>

namespace fif::core {
	class Entity final {
	  public:
		template <class T, class... Args> T *addComponent(Args &&...args) {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			std::unique_ptr<Component> &comp = m_Components.emplace_back(new T(args...));
			comp->mp_Entity = this;
			comp->onCreate();

			return static_cast<T *>(comp.get());
		}

		template <class T> T *getComponent() {
			FIF_PROFILE_FUNC();
			static_assert(std::is_base_of<Component, T>().value, "T is not a Component");

			for (std::unique_ptr<Component> &comp : m_Components) {
				T *c = dynamic_cast<T *>(comp.get());
				if (c != nullptr)
					return c;
			}

			return nullptr;
		}

		inline const std::vector<std::unique_ptr<Component>> &getComponents() const {
			return m_Components;
		}

		void update();
		void render();
		void onEvent(Event &event);

	  private:
		std::vector<std::unique_ptr<Component>> m_Components;
	};
} // namespace fif::core