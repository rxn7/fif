#pragma once

#include "fif/gfx/text/font.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct LabelComponent final {
		inline vec2 calculate_text_size() const {
			return Font::calculate_text_size(text, fontSize, fontSize * lineHeightFactor, fontSize * charSpacingFactor);
		}

		std::string text = "";
		f32 fontSize = 12.0f;
		f32 charSpacingFactor = 0.2f;
		f32 lineHeightFactor = 1.1f;
		Color color = Colors::BLACK;
		HorizontalTextAlign horizontalAlign = HorizontalTextAlign::LEFT;
		VerticalTextAlign verticalAlign = VerticalTextAlign::CENTER;

		// TODO: Font resource.
	};
}// namespace fif::gfx
