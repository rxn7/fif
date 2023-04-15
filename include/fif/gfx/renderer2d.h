#pragma once

#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/primitiveType.h"
#include "fif/gfx/rendererStats.h"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include <memory>

namespace fif::gfx {
	namespace Renderer2D {
		const RendererStats &getStats();
		OrthoCamera &getCamera();

		void init();
		void begin();
		void end();

		void renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle = 0.0f, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
		void renderCircle(const glm::vec2 &position, float diameter, const glm::vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f});
	}
}
