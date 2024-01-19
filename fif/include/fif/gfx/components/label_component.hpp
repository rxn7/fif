#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct LabelComponent final {
		i8 zIndex = 0;

		std::string text = "";
		f32 fontSize = 1.0f;
		Color color = Colors::BLACK;
		HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
		VerticalTextAlign verticalAlign = VerticalTextAlign::Center;
		std::shared_ptr<Font> p_font;
	};
}// namespace fif::gfx
