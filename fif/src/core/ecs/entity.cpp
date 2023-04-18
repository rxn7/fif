#include "fif/core/ecs/entity.h"

namespace fif::core {
	void Entity::update() {
		FIF_PROFILE_FUNC();
		for (std::unique_ptr<Component> &comp : m_Components)
			comp->onUpdate();
	}

	void Entity::render() {
		FIF_PROFILE_FUNC();
		for (std::unique_ptr<Component> &comp : m_Components)
			comp->onRender();
	}

	void Entity::onEvent(Event &event) {
		FIF_PROFILE_FUNC();
		for (std::unique_ptr<Component> &comp : m_Components)
			comp->onEvent(event);
	}
} // namespace fif::core
