#pragma once

#include "fif/core/ecs/component.h"
#include "fif/gfx/components/transformComponent.h"
#include "glm/vec4.hpp"

namespace fif::gfx {
	class RenderableComponent : public core::Component {
	  public:
		virtual void onCreate() override;
		virtual void onRender() override = 0;

	  public:
		glm::u8vec4 m_Color;
		TransformComponent *mp_Transform;

	  protected:
		RenderableComponent() {}
	};
} // namespace fif::gfx
