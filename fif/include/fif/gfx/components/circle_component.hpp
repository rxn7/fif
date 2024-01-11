#pragma once

#include "fif/gfx/color.hpp"

namespace fif::gfx {
	struct CircleComponent final {
		Color tint = Colors::WHITE;
		f32 radius = 50.0f;
	};
}// namespace fif::gfx
