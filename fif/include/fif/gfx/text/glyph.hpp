#pragma once

namespace fif::gfx {
	struct Glyph {
		vec2 offset;
		vec2 advance;
		vec2 startUv;
		vec2 endUv;
		u32vec2 size;
	};
}// namespace fif::gfx
