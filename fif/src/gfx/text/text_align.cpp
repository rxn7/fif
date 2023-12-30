#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	vec2 TextAlign::get_text_align_offset(const HorizontalTextAlign hAlign, const VerticalTextAlign vAlign, const vec2 textSize, const f32 fontSize) {
		vec2 offset = {};

		switch(hAlign) {
		case HorizontalTextAlign::CENTER:
			offset.x -= textSize.x * 0.5f;
			break;

		case HorizontalTextAlign::RIGHT:
			offset.x -= textSize.x;
			break;

		default:
			break;
		}

		switch(vAlign) {
		case VerticalTextAlign::TOP:
			offset.y -= fontSize;
			break;

		case VerticalTextAlign::CENTER:
			offset.y += textSize.y * 0.5f - fontSize;
			break;

		case VerticalTextAlign::BOTTOM:
			offset.y += textSize.y - fontSize;
			break;

		default:
			break;
		}

		return offset;
	}
}// namespace fif::gfx
