#include "fif/core/ecs/entity.hpp"

namespace fif::core {
	Entity::Entity(const std::string &name) : m_Name(name) {}

	void Entity::on_update() {
		FIF_PROFILE_FUNC();
		for(std::unique_ptr<Component> &comp : m_Components)
			comp->on_update();
	}

	void Entity::on_render() {
		FIF_PROFILE_FUNC();
		for(std::unique_ptr<Component> &comp : m_Components)
			comp->on_render();
	}

	void Entity::on_event(Event &event) {
		FIF_PROFILE_FUNC();
		for(std::unique_ptr<Component> &comp : m_Components)
			comp->on_event(event);
	}
}// namespace fif::core
