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
			// TODO: Should the radius be scaled by trans.scale.x or trans.scale.y or not at all?
			renderer.render_circle(trans.position, circle.radius, circle.tint);
		});

		registry.view<QuadComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, QuadComponent &quad, core::TransformComponent &trans) {
			renderer.render_quad(trans.position, quad.size * trans.scale, trans.angleRadians, quad.tint);
		});

		registry.view<SpriteComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, SpriteComponent &sprite, core::TransformComponent &trans) {
			if(!sprite.p_texture) {
				renderer.render_quad(trans.position, sprite.size * trans.scale, trans.angleRadians, sprite.tint);
				return;
			}

			renderer.render_sprite(sprite.p_texture, trans.position, sprite.size * trans.scale, trans.angleRadians, sprite.tint);
		});

		registry.view<LabelComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, LabelComponent &label, core::TransformComponent &trans) {
			// TODO: Add scale
			renderer.render_text(label.p_font ? label.p_font : Font::get_default(), trans.position, trans.scale, label.fontSize, label.text, label.color, label.verticalAlign, label.horizontalAlign);
		});
	}
}// namespace fif::gfx
