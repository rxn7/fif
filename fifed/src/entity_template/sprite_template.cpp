#include "sprite_template.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"

namespace fifed {
	void SpriteTemplate::create_components(EntityID ent, Scene &scene) {
		scene.add_component<TransformComponent>(ent);
		scene.add_component<SpriteComponent>(ent);
	}
}// namespace fifed
