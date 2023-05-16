#pragma once

#include "entity_template.hpp"

namespace fifed {
	class SpriteTemplate final : public EntityTemplate {
	public:
		ENTITY_TEMPLATE_NAME("Sprite")

	protected:
		void create_components(EntityID ent, Scene &scene) override;
	};
}// namespace fifed
