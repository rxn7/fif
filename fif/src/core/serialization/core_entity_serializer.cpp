#include "core_entity_serializer.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/components/uuid_component.hpp"
#include "fif/core/serialization/vec2_yaml.hpp"

namespace fif::core {
	void CoreEntitySerializer::serialize(core::Entity &entity, YAML::Emitter &emitter) {
		const UuidComponent &uuidComponent = entity.require_component<UuidComponent>();
		emitter << YAML::Key << "UUID" << YAML::Value << uuidComponent.uuid;

		if(TagComponent *tagComponent = entity.try_get_component<TagComponent>())
			emitter << YAML::Key << "Tag" << YAML::Value << tagComponent->tag;

		serialize_component<TransformComponent>(entity, emitter, [&emitter](TransformComponent &transformComponent) {
			emitter << YAML::Key << "Position" << YAML::Value << transformComponent.position;
			emitter << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;
			emitter << YAML::Key << "AngleRadians" << YAML::Value << transformComponent.angleRadians;
		});
	}

	void CoreEntitySerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		if(const YAML::Node uuidNode = entityNode["UUID"])
			entity.add_component<UuidComponent>().uuid = uuidNode.as<u64>();

		if(const YAML::Node tagNode = entityNode["Tag"])
			entity.add_component<TagComponent>().tag = tagNode.as<std::string>();

		try_get_component_node<TransformComponent>(entityNode, [&entity](const YAML::Node &transformComponentNode) {
			TransformComponent &transform = entity.add_component<TransformComponent>();
			transform.position = transformComponentNode["Position"].as<vec2>();
			transform.scale = transformComponentNode["Scale"].as<vec2>();
			transform.angleRadians = transformComponentNode["AngleRadians"].as<f32>();
		});
	}
}// namespace fif::core
