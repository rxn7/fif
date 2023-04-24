#pragma once

#include "fif/gfx/components/transform_component.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	class RenderableComponent : public core::Component {
	public:
		virtual void on_create() override;
		virtual void on_render() override = 0;

	public:
		glm::u8vec4 m_Color;
		TransformComponent *mp_Transform;

	protected:
		RenderableComponent() {}
	};
}// namespace fif::gfx
