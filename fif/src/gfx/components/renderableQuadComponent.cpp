#include "fif/gfx/components/renderableQuadComponent.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	void RenderableQuadComponent::onRender() {
		if(mp_Transform->m_Angle == 0) {
			Renderer2D::renderQuad(mp_Transform->m_Position, m_Size, m_Color);
		} else {
			Renderer2D::renderQuadRotated(mp_Transform->m_Position, m_Size, mp_Transform->m_Angle, m_Color);
		}
	}
}// namespace fif::gfx
