#include "fif/gfx/components/renderable_circle_component.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	void RenderableCircleComponent::on_render() {
		Renderer2D::render_circle_frag(mp_Transform->m_Position, m_Radius, m_Color);
	}
}// namespace fif::gfx
