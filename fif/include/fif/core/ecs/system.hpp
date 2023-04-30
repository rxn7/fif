#pragma once

#include "fif/core/event/event.hpp"
#include "fif/core/types.hpp"

#include "entt/entt.hpp"
#include <functional>

namespace fif::core {
	typedef std::function<void(entt::registry &registry)> RenderSystem;
	typedef std::function<void(entt::registry &registry, f32 dt)> UpdateSystem;
	typedef std::function<void(entt::registry &registry, Event &event)> EventSystem;
}// namespace fif::core
