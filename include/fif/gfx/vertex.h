#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::u8vec4 color;
	};
}
