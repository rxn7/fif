#pragma once

#include "fif/gfx/components/renderableComponent.hpp"

namespace fif::gfx {
	class RenderableQuadComponent final : public RenderableComponent {
	  public:
		COMPONENT_NAME(RenderableQuadComponent);

		void onRender() override;

	  public:
		glm::vec2 m_Size{50.0f, 50.0f};
	};
} // namespace fif::gfx
