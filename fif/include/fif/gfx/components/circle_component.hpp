#pragma once

namespace fif::gfx {
	struct CircleComponent final {
		Color tint = Colors::WHITE;
		f32 radius = 50.0f;
		u16 segments = 16;
	};
}// namespace fif::gfx
