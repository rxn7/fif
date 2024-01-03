#pragma once

#include "entity_template.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"

namespace fifed {
	class SpriteTemplate final : public EntityTemplate {
	public:
		SpriteTemplate() = default;
		ENTITY_TEMPLATE_NAME("Sprite")

	protected:
		inline void create_components(Entity &ent) override {
			ent.add_component<TransformComponent>();
			ent.add_component<SpriteComponent>();
		}
	};
}// namespace fifed
