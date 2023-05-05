#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/renderable_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fif::gfx {
	void renderer_system(entt::registry &registry) {
		Renderer2D &renderer = GfxModule::get_instance()->get_renderer2D();

		registry.view<TransformComponent, RenderableComponent, CircleComponent>().each([&]([[maybe_unused]] core::EntityID entity, auto &trans, auto &renderable, auto &circle) {
			if(circle.segments == 0)
				renderer.render_circle_frag(trans.position, circle.radius, renderable.color);
			else
				renderer.render_circle(trans.position, circle.radius, circle.segments, renderable.color);
		});

		registry.view<TransformComponent, RenderableComponent, QuadComponent>().each([&]([[maybe_unused]] core::EntityID entity, auto &trans, auto &renderable, auto &quad) {
			renderer.render_quad(trans.position, quad.size, trans.angle, renderable.color);
		});
	}
}// namespace fif::gfx
