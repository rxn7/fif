#pragma once

#include "entity_template.hpp"

namespace fifed {
	class SpriteTemplate final : public EntityTemplate {
	public:
		ENTITY_TEMPLATE_NAME("Sprite")

	protected:
		void create_components(Entity &ent) override;
	};
}// namespace fifed
