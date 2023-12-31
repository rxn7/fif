#pragma once

namespace fif::gfx {
	enum class HorizontalTextAlign { LEFT, CENTER, RIGHT };
	enum class VerticalTextAlign { TOP, CENTER, BOTTOM };

	namespace TextAlign {
		vec2 get_text_align_offset(const HorizontalTextAlign hAlign, const VerticalTextAlign vAlign, const vec2 textSize, const f32 fontSize);
	}
}// namespace fif::gfx
