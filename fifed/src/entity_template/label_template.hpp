#pragma once

#include "entity_template.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/label_component.hpp"

namespace fifed {
	class LabelTemplate final : public EntityTemplate {
	public:
		LabelTemplate() = default;
		ENTITY_TEMPLATE_NAME("Label")

	protected:
		inline void create_components(Entity &ent) override {
			ent.add_component<TransformComponent>();
			ent.add_component<LabelComponent>();
		}
	};
}// namespace fifed
