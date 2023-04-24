#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/orthoCamera.hpp"

#include <memory>

namespace fif::gfx {
	namespace Renderer2D {
		struct Stats {
			u32 circles = 0;
			u32 quads = 0;
			u32 vertices = 0;
			u32 elements = 0;
		};

		OrthoCamera &getCamera();
		Stats &getStats();

		void init();
		void begin();
		void end();

		void renderQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::u8vec4 &color = {255, 255, 255, 255});

		void renderQuadRotated(const glm::vec2 &position, const glm::vec2 &size, float angle = 0.0f, const glm::u8vec4 &color = {255, 255, 255, 255});

		void renderCircle(const glm::vec2 &position, float radius, u16 segmentCount = 0, const glm::u8vec4 &color = {255, 255, 255, 255});

		void renderCircleFrag(const glm::vec2 &position, float radius, const glm::u8vec4 &color = {255, 255, 255, 255});
	} // namespace Renderer2D
} // namespace fif::gfx
