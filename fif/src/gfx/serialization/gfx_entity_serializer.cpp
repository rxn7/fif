#include "./gfx_entity_serializer.hpp"
#include "fif/core/project.hpp"
#include "fif/core/serialization/vec2_yaml.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/label_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/serialization/color_yaml.hpp"
#include "fif/gfx/text/text_align.hpp"

#include <filesystem>

namespace fif::gfx {
	void GfxEntitySerializer::serialize(core::Entity &entity, YAML::Emitter &emitter) {
		serialize_component<SpriteComponent>(entity, emitter, [&emitter](SpriteComponent &spriteComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << spriteComponent.tint;
			emitter << YAML::Key << "Size" << YAML::Value << spriteComponent.size;
			emitter << YAML::Key << "TextureUUID" << YAML::Value << (spriteComponent.p_texture ? static_cast<u64>(spriteComponent.p_texture->get_uuid()) : 0u);
		});

		serialize_component<QuadComponent>(entity, emitter, [&emitter](QuadComponent &quadComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << quadComponent.tint;
			emitter << YAML::Key << "Size" << YAML::Value << quadComponent.size;
		});

		serialize_component<CircleComponent>(entity, emitter, [&emitter](CircleComponent &circleComponent) {
			emitter << YAML::Key << "Tint" << YAML::Value << circleComponent.tint;
			emitter << YAML::Key << "Radius" << YAML::Value << circleComponent.radius;
		});

		serialize_component<LabelComponent>(entity, emitter, [&emitter](LabelComponent &labelComponent) {
			emitter << YAML::Key << "Text" << YAML::Value << labelComponent.text;
			emitter << YAML::Key << "Size" << YAML::Value << labelComponent.fontSize;
			emitter << YAML::Key << "Color" << YAML::Value << labelComponent.color;
			emitter << YAML::Key << "HorizontalAlign" << YAML::Value << static_cast<int>(labelComponent.horizontalAlign);
			emitter << YAML::Key << "VerticalAlign" << YAML::Value << static_cast<int>(labelComponent.verticalAlign);
			emitter << YAML::Key << "FontUUID" << YAML::Value << (labelComponent.p_font ? static_cast<u64>(labelComponent.p_font->get_uuid()) : 0u);
		});
	}

	void GfxEntitySerializer::deserialize(core::Entity &entity, const YAML::Node &entityNode) {
		try_get_component_node<SpriteComponent>(entityNode, [&entity](const YAML::Node &spriteComponentNode) {
			SpriteComponent &spriteComponent = entity.require_component<SpriteComponent>();
			spriteComponent.size = spriteComponentNode["Size"].as<vec2>();
			spriteComponent.tint = spriteComponentNode["Tint"].as<Color>();

			const core::UUID textureUUID = spriteComponentNode["TextureUUID"].as<u64>();
			if(textureUUID != 0u)
				spriteComponent.p_texture = core::Project::get_resource_manager().get_resource<Texture>(textureUUID);
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

		try_get_component_node<LabelComponent>(entityNode, [&entity](const YAML::Node &labelComponentNode) {
			LabelComponent &labelComponent = entity.require_component<LabelComponent>();
			labelComponent.text = labelComponentNode["Text"].as<std::string>();
			labelComponent.fontSize = labelComponentNode["Size"].as<f32>();
			labelComponent.color = labelComponentNode["Color"].as<Color>();
			labelComponent.horizontalAlign = static_cast<HorizontalTextAlign>(labelComponentNode["HorizontalAlign"].as<int>());
			labelComponent.verticalAlign = static_cast<VerticalTextAlign>(labelComponentNode["VerticalAlign"].as<int>());
			core::UUID uuid = labelComponentNode["FontUUID"].as<u64>();
			if(uuid != 0u)
				labelComponent.p_font = core::Project::get_resource_manager().get_resource<Font>(uuid);
		});
	}
}// namespace fif::gfx
