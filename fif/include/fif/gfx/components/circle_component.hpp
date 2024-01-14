#pragma once

#include "fif/gfx/color.hpp"

namespace fif::gfx {
	struct CircleComponent final {
		i8 zIndex = 0;

		Color tint = Colors::WHITE;
		f32 radius = 50.0f;
	};
}// namespace fif::gfx
