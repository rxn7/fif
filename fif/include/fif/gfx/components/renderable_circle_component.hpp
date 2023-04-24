#pragma once

#include "fif/gfx/components/renderable_component.hpp"

namespace fif::gfx {
	class RenderableCircleComponent final : public RenderableComponent {
	public:
		COMPONENT_NAME(RenderableCircleComponent);

		void on_render() override;

	public:
		float m_Radius = 50.0f;
	};
}// namespace fif::gfx
