#pragma once

#include "fif/gfx/text/font.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct LabelComponent final {
		std::string text = "";
		f32 fontSize = 1.0f;
		Color color = Colors::BLACK;
		HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
		VerticalTextAlign verticalAlign = VerticalTextAlign::Center;

		// TODO: Font resource.
	};
}// namespace fif::gfx
