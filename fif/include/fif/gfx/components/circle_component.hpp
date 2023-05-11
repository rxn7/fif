#pragma once

namespace fif::gfx {
	struct CircleComponent final {
		Color tint = Colors::WHITE;
		f32 radius = 100.0f;
		u16 segments = 0;
	};
}// namespace fif::gfx
