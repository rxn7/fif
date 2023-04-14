#pragma once

#include "fif/gfx/primitiveType.h"
#include "fif/gfx/rendererStats.h"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace fif::gfx {
	namespace Renderer2D {
		const RendererStats &getStats();

		void init();
		void begin();
		void end();

		void renderQuad(const glm::vec2 &position, const glm::vec2 &size = {10.0f, 10.0f}, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
		void renderCircle(const glm::vec2 &position, float radius = 5, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
	}
}
