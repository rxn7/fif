#include "entity_template.hpp"

namespace fifed {
	EntityID EntityTemplate::create(Scene &scene) {
		EntityID ent = scene.create_entity(get_name().data());
		create_components(ent, scene);
		return ent;
	}
}// namespace fifed
