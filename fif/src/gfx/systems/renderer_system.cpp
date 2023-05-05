#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/renderable_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	void renderer_system(entt::registry &registry) {
		registry.view<TransformComponent, RenderableComponent, CircleComponent>().each([]([[maybe_unused]] core::EntityID entity, auto &trans, auto &renderable, auto &circle) {
			Renderer2D::render_circle_frag(trans.position, circle.radius, renderable.color);
		});

		registry.view<TransformComponent, RenderableComponent, QuadComponent>().each([]([[maybe_unused]] core::EntityID entity, auto &trans, auto &renderable, auto &quad) {
			Renderer2D::render_quad(trans.position, quad.size, trans.angle, renderable.color);
		});
	}
}// namespace fif::gfx
