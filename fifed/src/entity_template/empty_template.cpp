#include "empty_template.hpp"
#include "fif/core/ecs/components/transform_component.hpp"

namespace fifed {
	void EmptyTemplate::create_components(EntityID ent, Scene &scene) {
		scene.add_component<TransformComponent>(ent);
	}
}// namespace fifed
