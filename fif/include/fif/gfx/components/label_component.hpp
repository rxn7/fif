#pragma once

#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct LabelComponent final {
		std::string text = "";
		f32 size = 12.0f;
		Color color = Colors::BLACK;
		HorizontalTextAlign horizontalAlign = HorizontalTextAlign::LEFT;
		VerticalTextAlign verticalAlign = VerticalTextAlign::CENTER;

		// TODO: Font resource.
	};
}// namespace fif::gfx
