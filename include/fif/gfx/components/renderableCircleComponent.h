#pragma once

#include "fif/gfx/components/renderableComponent.h"

namespace fif::gfx {
	class RenderableCircleComponent final : public RenderableComponent {
	  public:
		COMPONENT_NAME(RenderableCircleComponent);

		void onRender() override;

	  public:
		float m_Radius = 50.0f;
	};
} // namespace fif::gfx
