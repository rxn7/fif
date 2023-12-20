#pragma once

#include "entity_template.hpp"

namespace fifed {
	class EmptyTemplate final : public EntityTemplate {
	public:
		ENTITY_TEMPLATE_NAME("Empty")

	protected:
		void create_components(Entity &ent) override;
	};
}// namespace fifed
