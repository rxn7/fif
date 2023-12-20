#include "sprite_template.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"

namespace fifed {
	void SpriteTemplate::create_components(Entity &ent) {
		ent.add_component<TransformComponent>();
		ent.add_component<SpriteComponent>();
	}
}// namespace fifed
