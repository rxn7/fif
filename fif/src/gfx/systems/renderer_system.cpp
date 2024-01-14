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
			renderer.add_render_command<CircleRenderCommand>(circle.zIndex, trans.position, circle.radius, circle.tint);
		});

		registry.view<QuadComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, QuadComponent &quad, core::TransformComponent &trans) {
			renderer.add_render_command<QuadRenderCommand>(quad.zIndex, trans.position, trans.scale * quad.size, trans.angleRadians, quad.tint);
		});

		registry.view<SpriteComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, SpriteComponent &sprite, core::TransformComponent &trans) {
			if(!sprite.p_Texture) {
				renderer.add_render_command<QuadRenderCommand>(sprite.zIndex, trans.position, trans.scale * sprite.size, trans.angleRadians, sprite.tint);
				return;
			}

			renderer.add_render_command<SpriteRenderCommand>(sprite.zIndex, trans.position, trans.scale * sprite.size, trans.angleRadians, sprite.tint, sprite.p_Texture);
		});

		registry.view<LabelComponent, core::TransformComponent>().each([&]([[maybe_unused]] core::EntityID entity, LabelComponent &label, core::TransformComponent &trans) {
			renderer.add_render_command<TextRenderCommand>(
				label.zIndex, label.text, label.p_font ? *label.p_font : Font::get_default(), trans.position, trans.scale * label.fontSize, label.color, label.verticalAlign, label.horizontalAlign);
		});
	}
}// namespace fif::gfx
