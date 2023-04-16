#pragma once

#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/primitiveType.h"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#include <memory>

namespace fif::gfx {
	namespace Renderer2D {
		OrthoCamera &getCamera();

		void init();
		void begin();
		void end();

		void renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle = 0.0f, const glm::u8vec4 &color = {255,255,255,255});

		void renderCircleTriangle(const glm::vec2 &position, float diameter, std::uint16_t segmentCount = 0, const glm::u8vec4 &color = {255,255,255,255});
		void renderCircleFrag(const glm::vec2 &position, float diameter, const glm::u8vec4 &color = {255,255,255,255});
	}
}
