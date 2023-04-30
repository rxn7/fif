#include "fif/core/ecs/scene.hpp"
#include "fif/gfx/components/circle_component.hpp"
#include "fif/gfx/components/quad_component.hpp"
#include "fif/gfx/components/renderable_component.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "fif/gfx/renderer2d.hpp"

#include "glm/gtc/constants.hpp"

namespace fif::gfx {
	void renderer_system(entt::registry &registry) {
		registry.view<TransformComponent, RenderableComponent, CircleComponent>().each([](core::EntityID entity, TransformComponent &trans, RenderableComponent &renderable, CircleComponent &circle) {
			Renderer2D::render_circle_frag(trans.position, circle.radius, renderable.color);
		});

		registry.view<TransformComponent, RenderableComponent, QuadComponent>().each([](core::EntityID entity, TransformComponent &trans, RenderableComponent &renderable, QuadComponent &quad) {
			if(trans.angle != 0)
				Renderer2D::render_quad_rotated(trans.position, quad.size, trans.angle, renderable.color);
			else
				Renderer2D::render_quad(trans.position, quad.size, renderable.color);
		});
	}
}// namespace fif::gfx
