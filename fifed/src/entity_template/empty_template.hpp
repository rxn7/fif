#pragma once

#include "entity_template.hpp"

namespace fifed {
	class EmptyTemplate final : public EntityTemplate {
	public:
		ENTITY_TEMPLATE_NAME("Empty")

	protected:
		void create_components(EntityID ent, Scene &scene) override;
	};
}// namespace fifed
