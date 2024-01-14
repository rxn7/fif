#include "renderer_system.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/label_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fif::gfx {
	void renderer_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		Renderer2D &renderer = GfxModule::get_instance().get_renderer2D();

		registry.view<CircleComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, CircleComponent &circle, core::TransformComponent &trans) {
			const CircleRenderCommand cmd = {.position = trans.position, .radius = circle.radius, .color = circle.tint};
			renderer.render_circle(cmd);
		});

		registry.view<QuadComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, QuadComponent &quad, core::TransformComponent &trans) {
			const QuadRenderCommand cmd = {
				.position = trans.position,
				.size = trans.scale * quad.size,
				.angle = trans.angleRadians,
				.color = quad.tint,
			};
			renderer.render_quad(cmd);
		});

		registry.view<SpriteComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, SpriteComponent &sprite, core::TransformComponent &trans) {
			if(!sprite.p_Texture) {
				const QuadRenderCommand cmd = {
					.position = trans.position,
					.size = trans.scale * sprite.size,
					.angle = trans.angleRadians,
					.color = sprite.tint,
				};
				renderer.render_quad(cmd);
				return;
			}

			const SpriteRenderCommand cmd = {
				.position = trans.position,
				.size = trans.scale * sprite.size,
				.angle = trans.angleRadians,
				.color = sprite.tint,
				.p_Texture = sprite.p_Texture,
			};

			renderer.render_sprite(cmd);
		});

		registry.view<LabelComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, LabelComponent &label, core::TransformComponent &trans) {
			const TextRenderCommand cmd = {.text = label.text,
										   .font = label.p_font ? *label.p_font : Font::get_default(),
										   .position = trans.position,
										   .size = trans.scale * label.fontSize,
										   .color = label.color,
										   .vAlign = label.verticalAlign,
										   .hAlign = label.horizontalAlign};
			// TODO: Add scale
			renderer.render_text(cmd);
		});
	}
}// namespace fif::gfx
