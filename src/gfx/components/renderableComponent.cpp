#include "fif/gfx/components/renderableComponent.h"

#include "fif/core/ecs/entity.h"
#include "fif/core/assertion.h"

namespace fif::gfx {
	void RenderableComponent::onCreate() {
		mp_Transform = mp_Entity->getComponent<TransformComponent>();
		FIF_ASSERT(mp_Transform != nullptr, "Entity with RenderableComponent must add TransformComponent first");
	}
}
