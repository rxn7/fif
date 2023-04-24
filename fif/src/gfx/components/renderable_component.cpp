#include "fif/core/ecs/entity.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/gfx/components/renderable_component.hpp"

namespace fif::gfx {
	void RenderableComponent::on_create() {
		mp_Transform = mp_Entity->get_component<TransformComponent>();
		FIF_ASSERT(mp_Transform != nullptr, "Entity with RenderableComponent must add TransformComponent first");
	}
}// namespace fif::gfx
