#pragma once

#include "glm/vec2.hpp"

namespace fif::gfx {
	struct TransformComponent {
		glm::vec2 position;
		f32 angle;
	};
}// namespace fif::gfx
