#pragma once

#include "fif/core/event/event.h"
#include <cinttypes>

namespace fif::core {
	class Layer {
	public:
		Layer() = default;
		virtual ~Layer() = default;

		inline constexpr bool operator<(const Layer &other) { return getZIndex() < other.getZIndex(); }

		virtual void update([[maybe_unused]] float dt) = 0;
		virtual void renderImGui() = 0;
		virtual void render() = 0;
		virtual void onEvent(Event &event) = 0;;

		virtual constexpr std::uint8_t getZIndex() const = 0;
	};
}
