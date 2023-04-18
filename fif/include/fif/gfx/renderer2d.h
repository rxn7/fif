#pragma once

#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/primitiveType.h"

#include <memory>

namespace fif::gfx {
	namespace Renderer2D {
		struct Stats {
			std::uint32_t circles = 0;
			std::uint32_t quads = 0;
			std::uint32_t vertices = 0;
			std::uint32_t elements = 0;
		};

		OrthoCamera &getCamera();
		Stats &getStats();

		void init();
		void begin();
		void end();

		void renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle = 0.0f, const glm::u8vec4 &color = {255, 255, 255, 255});

		void renderCircleTriangle(const glm::vec2 &position, float radius, std::uint16_t segmentCount = 0, const glm::u8vec4 &color = {255, 255, 255, 255});
		void renderCircleFrag(const glm::vec2 &position, float radius, const glm::u8vec4 &color = {255, 255, 255, 255});
	} // namespace Renderer2D
} // namespace fif::gfx
