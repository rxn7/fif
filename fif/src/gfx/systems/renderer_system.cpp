#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/sprite_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fif::gfx {
	void renderer_system([[maybe_unused]] const core::ApplicationStatus &status, entt::registry &registry) {
		Renderer2D &renderer = GfxModule::get_instance()->get_renderer2D();

		registry.view<TransformComponent, CircleComponent>().each([&]([[maybe_unused]] core::EntityID entity, TransformComponent &trans, CircleComponent &circle) {
			if(circle.segments == 0)
				renderer.render_circle_frag(trans.position, circle.radius, circle.tint);
			else
				renderer.render_circle(trans.position, circle.radius, circle.segments, circle.tint);
		});

		registry.view<TransformComponent, QuadComponent>().each([&]([[maybe_unused]] core::EntityID entity, TransformComponent &trans, QuadComponent &quad) {
			renderer.render_quad(trans.position, quad.size, trans.angle, quad.tint);
		});

		registry.view<TransformComponent, SpriteComponent>().each([&]([[maybe_unused]] core::EntityID entity, TransformComponent &trans, SpriteComponent &sprite) {
			if(!sprite.p_texture)
				return;

			renderer.render_sprite(sprite.p_texture, trans.position, sprite.size, trans.angle, sprite.tint);
		});
	}
}// namespace fif::gfx
