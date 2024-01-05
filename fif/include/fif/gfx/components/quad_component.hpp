#pragma once

#include "fif/gfx/color.hpp"

namespace fif::gfx {
	struct QuadComponent final {
		Color tint = Colors::WHITE;
		vec2 size = {100.0f, 100.0f};
	};
}// namespace fif::gfx
