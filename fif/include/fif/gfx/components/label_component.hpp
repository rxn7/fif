#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	struct LabelComponent final {
		std::string text = "";
		f32 fontSize = 1.0f;
		Color color = Colors::BLACK;
		HorizontalTextAlign horizontalAlign = HorizontalTextAlign::Left;
		VerticalTextAlign verticalAlign = VerticalTextAlign::Center;

		// TODO: Use the default font. (we would need to copy it to the runtime in the future)
		std::shared_ptr<Font> p_font;
	};
}// namespace fif::gfx
