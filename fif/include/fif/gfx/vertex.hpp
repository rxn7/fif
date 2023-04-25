#pragma once

#include "fif/gfx/color.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	struct Vertex final {
		glm::vec2 position = {0, 0};
		glm::vec2 uv = {0, 0};
		Color color = {255, 255, 255, 255};
	};
}// namespace fif::gfx
