#include "empty_template.hpp"
#include "fif/core/ecs/components/transform_component.hpp"

namespace fifed {
	void EmptyTemplate::create_components(Entity &ent) {
		ent.add_component<TransformComponent>();
	}
}// namespace fifed
