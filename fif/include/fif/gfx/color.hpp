#pragma once

#include "fif/core/types.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	typedef glm::u8vec4 Color;

	inline glm::vec4 get_normalized_color(const Color &color) {
		return glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	}

	inline Color get_color_from_normalized(const glm::vec4 &color) {
		return Color(static_cast<u8>(color.r * 255.0f), static_cast<u8>(color.g * 255.0f), static_cast<u8>(color.b * 255.0f), static_cast<u8>(color.a * 255.0f));
	}
}// namespace fif::gfx
