#pragma once

#include "fif/gfx/components/renderable_component.hpp"

#include "glm/vec2.hpp"

namespace fif::gfx {
	struct QuadComponent {
		glm::vec2 size = {50.0f, 50.0f};
	};
}// namespace fif::gfx