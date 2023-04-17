#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	struct Vertex final {
		glm::vec2 position = {0,0};
		glm::vec2 uv = {0,0};
		glm::u8vec4 color = {255,255,255,255};
	};
}
