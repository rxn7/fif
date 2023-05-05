#pragma once

#include "fif/gfx/color.hpp"

namespace fif::gfx {
	struct Vertex final {
		glm::vec2 position = {0, 0};
		glm::vec2 uv = {0, 0};
		Color color = {255, 255, 255, 255};
	};
}// namespace fif::gfx
