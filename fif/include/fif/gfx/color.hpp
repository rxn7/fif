#pragma once

#include "glm/vec4.hpp"

namespace fif::gfx {
	typedef glm::u8vec4 Color;

	inline glm::vec4 get_normalized_color(const Color &color) {
		return glm::vec4(color) * (1.0f / 255.0f);
	}

	inline Color get_color_from_normalized(const glm::vec4 &color) {
		return Color(color * 255.0f);
	}
}// namespace fif::gfx
