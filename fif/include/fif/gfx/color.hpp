#pragma once

#include "glm/vec4.hpp"

namespace fif::gfx {
	typedef glm::u8vec4 Color;
	typedef glm::u8vec3 Color3;

	inline glm::vec4 normalize_color(const Color &color) {
		return glm::vec4(color) * (1.0f / 255.0f);
	}

	inline glm::vec3 normalize_color3(const Color3 &color) {
		return glm::vec3(color) * (1.0f / 255.0f);
	}

	inline Color denormalize_color(const glm::vec4 &color) {
		return Color(color * 255.0f);
	}

	inline Color3 denormalize_color3(const glm::vec3 &color) {
		return Color3(color * 255.0f);
	}

#define DEFINE_COLOR(name, r, g, b)                                                                                                                  \
	inline constexpr Color name { r, g, b, 255 }

	namespace Colors {
		DEFINE_COLOR(WHITE, 255, 255, 255);
		DEFINE_COLOR(RED, 255, 0, 0);
		DEFINE_COLOR(GREEN, 0, 255, 0);
		DEFINE_COLOR(BLUE, 0, 0, 255);
		DEFINE_COLOR(BLACK, 255, 255, 255);
	};// namespace Colors
}// namespace fif::gfx
