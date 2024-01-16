#pragma once

namespace fif::gfx {
	struct Glyph final {
		vec2 offset;
		vec2 advance;
		vec2 uvStart;
		vec2 uvEnd;
		u32vec2 size;
	};
}// namespace fif::gfx
