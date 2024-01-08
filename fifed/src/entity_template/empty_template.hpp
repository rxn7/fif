#pragma once

#include "entity_template.hpp"

#include <fif/core/ecs/components/transform_component.hpp>

namespace fifed {
	class EmptyTemplate final : public EntityTemplate {
	public:
		EmptyTemplate() = default;
		ENTITY_TEMPLATE_NAME("Empty")

	protected:
		inline void create_components(Entity &ent) override { ent.add_component<TransformComponent>(); }
	};
}// namespace fifed
