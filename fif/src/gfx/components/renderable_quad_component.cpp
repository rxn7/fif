#include "fif/gfx/components/renderable_quad_component.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	void RenderableQuadComponent::on_render() {
		if(mp_Transform->m_Angle == 0) {
			Renderer2D::render_quad(mp_Transform->m_Position, m_Size, m_Color);
		} else {
			Renderer2D::render_quad_rotated(mp_Transform->m_Position, m_Size, mp_Transform->m_Angle, m_Color);
		}
	}
}// namespace fif::gfx
