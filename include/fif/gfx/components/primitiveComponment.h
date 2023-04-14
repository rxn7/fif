#pragma once

#include "fif/gfx/primitiveType.h"
#include "glm/vec4.hpp"

namespace fif::gfx {
	struct PrimitiveComponent {
		PrimitiveType type;
		glm::vec4 color;
	};
}
