#include ".//core_serializer.hpp"

#include "fif/core/ecs/components/tag_component.hpp"
#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/serialization/vec2_yaml.hpp"

namespace fif::core {
	void CoreSerializer::serialize(const core::Entity &entity, YAML::Emitter &emitter) {
		serialize_component<TagComponent>(entity, emitter, [&emitter](TagComponent &tagComponent) { emitter << YAML::Key << "Tag" << YAML::Value << tagComponent.tag; });

		serialize_component<TransformComponent>(entity, emitter, [&emitter](TransformComponent &transformComponent) {
			emitter << YAML::Key << "Position" << YAML::Value << transformComponent.position;
			emitter << YAML::Key << "Scale" << YAML::Value << transformComponent.scale;
			emitter << YAML::Key << "AngleRadians" << YAML::Value << transformComponent.angleRadians;
		});
	}

	void CoreSerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		try_get_component_node<TagComponent>(entityNode, [&entity](const YAML::Node &tagComponentNode) { entity.add_component<TagComponent>().tag = tagComponentNode["Tag"].as<std::string>(); });
		try_get_component_node<TransformComponent>(entityNode, [&entity](const YAML::Node &transformComponentNode) {
			TransformComponent &transform = entity.add_component<TransformComponent>();
			transform.position = transformComponentNode["Position"].as<vec2>();
			transform.scale = transformComponentNode["Scale"].as<vec2>();
			transform.angleRadians = transformComponentNode["AngleRadians"].as<f32>();
		});
	}
}// namespace fif::core
