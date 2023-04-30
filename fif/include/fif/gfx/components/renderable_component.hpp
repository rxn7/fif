#pragma once

#include "fif/gfx/color.hpp"

#include "glm/vec4.hpp"

namespace fif::gfx {
	struct RenderableComponent {
		Color color = {255, 255, 255, 255};
	};
}// namespace fif::gfx
