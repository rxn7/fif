#include "fif/gfx/components/renderableComponent.hpp"
#include "fif/core/ecs/entity.hpp"
#include "fif/core/util/assertion.hpp"

namespace fif::gfx {
	void RenderableComponent::onCreate() {
		mp_Transform = mp_Entity->getComponent<TransformComponent>();
		FIF_ASSERT(mp_Transform != nullptr, "Entity with RenderableComponent must add "
											"TransformComponent first");
	}
}// namespace fif::gfx
