#include "fif/gfx/text/text_align.hpp"

namespace fif::gfx {
	vec2 TextAlign::get_text_align_offset(const HorizontalTextAlign hAlign, const VerticalTextAlign vAlign, const vec2 textSize, const f32 fontHeight) {
		vec2 offset = {};

		switch(hAlign) {
		case HorizontalTextAlign::Center:
			offset.x -= textSize.x * 0.5f;
			break;

		case HorizontalTextAlign::Right:
			offset.x -= textSize.x;
			break;

		default:
			break;
		}

		switch(vAlign) {
		case VerticalTextAlign::Top:
			offset.y -= fontHeight * 0.5f;
			break;

		case VerticalTextAlign::Center:
			offset.y += textSize.y * 0.5f - fontHeight * 0.75f;
			break;

		case VerticalTextAlign::Bottom:
			offset.y += textSize.y - fontHeight;
			break;

		default:
			break;
		}

		return offset;
	}
}// namespace fif::gfx
