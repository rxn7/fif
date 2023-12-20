#include "entity_template.hpp"

namespace fifed {
	EntityID EntityTemplate::create(Scene &scene) {
		Entity ent = Entity(scene, scene.create_entity(get_name().data()));
		create_components(ent);
		return ent.m_ID;
	}
}// namespace fifed
