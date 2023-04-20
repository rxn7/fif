#include "fif/gfx/components/renderableQuadComponent.h"
#include "fif/gfx/renderer2d.h"

namespace fif::gfx {
	void RenderableQuadComponent::onRender() {
		Renderer2D::renderQuad(mp_Transform->m_Position, m_Size, mp_Transform->m_Angle, m_Color);
	}
} // namespace fif::gfx
