#include "fif/gfx/components/renderableCircleComponent.h"
#include "fif/gfx/renderer2d.h"

namespace fif::gfx {
	void RenderableCircleComponent::onRender() {
		Renderer2D::renderCircleFrag(mp_Transform->m_Position, m_Radius, m_Color);
	}
} // namespace fif::gfx