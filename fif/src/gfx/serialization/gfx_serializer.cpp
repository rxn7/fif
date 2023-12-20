#include "./gfx_serializer.hpp"
#include "fif/core/ecs/serialization/vec2_yaml.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/serialization/color_yaml.hpp"

namespace fif::gfx {
	void GfxSerializer::serialize(const core::Entity &entity, YAML::Emitter &emitter) {
		serialize_component<SpriteComponent>(entity, emitter, [&emitter](SpriteComponent &spriteComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << spriteComponent.tint;
			emitter << YAML::Key << "Size" << YAML::Value << spriteComponent.size;
			emitter << YAML::Key << "TexturePath" << YAML::Value << (spriteComponent.p_texture != nullptr ? spriteComponent.p_texture->m_Path : "");
		});

		serialize_component<QuadComponent>(entity, emitter, [&emitter](QuadComponent &quadComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << quadComponent.tint;
			emitter << YAML::Key << "Size" << YAML::Value << quadComponent.size;
		});

		serialize_component<CircleComponent>(entity, emitter, [&emitter](CircleComponent &circleComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << circleComponent.tint;
			emitter << YAML::Key << "Radius" << YAML::Value << circleComponent.radius;
		});
	}

	void GfxSerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		try_get_component_node<SpriteComponent>(entityNode, [&entity](const YAML::Node &spriteComponentNode) {
			SpriteComponent &spriteComponent = entity.require_component<SpriteComponent>();
			spriteComponent.size = spriteComponentNode["Size"].as<vec2>();
			spriteComponent.tint = spriteComponentNode["Tint"].as<Color>();

			const std::string texturePath = spriteComponentNode["TexturePath"].as<std::string>();
			if(!texturePath.empty())
				spriteComponent.set_texture(std::make_shared<Texture>(texturePath));
		});

		try_get_component_node<QuadComponent>(entityNode, [&entity](const YAML::Node &quadComponentNode) {
			QuadComponent &quadComponent = entity.require_component<QuadComponent>();
			quadComponent.size = quadComponentNode["Size"].as<vec2>();
			quadComponent.tint = quadComponentNode["Tint"].as<Color>();
		});

		try_get_component_node<CircleComponent>(entityNode, [&entity](const YAML::Node &circleComponentNode) {
			CircleComponent &circleComponent = entity.require_component<CircleComponent>();
			circleComponent.tint = circleComponentNode["Tint"].as<Color>();
			circleComponent.radius = circleComponentNode["Radius"].as<f32>();
		});
	}
}// namespace fif::gfx
