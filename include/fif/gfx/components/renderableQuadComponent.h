#pragma once

#include "fif/gfx/components/renderableComponent.h"

namespace fif::gfx {
	class RenderableQuadComponent final : public RenderableComponent {
	public:
		COMPONENT_NAME(RenderableQuadComponent);

		void onRender() override;

	public:
		glm::vec2 m_Size;
	};
}
