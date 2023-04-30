#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/ortho_camera.hpp"

#include <memory>

namespace fif::gfx {
	namespace Renderer2D {
		struct Stats {
			u32 circles = 0;
			u32 quads = 0;
			u32 vertices = 0;
			u32 elements = 0;
		};

		OrthoCamera &get_camera();
		Stats &get_stats();

		void init();
		void begin();
		void end();

		void render_quad(const glm::vec2 &position, const glm::vec2 &size, const glm::u8vec4 &color = {255, 255, 255, 255});

		void render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle = 0.0f, const glm::u8vec4 &color = {255, 255, 255, 255});

		void render_circle(const glm::vec2 &position, f32 radius, u16 segmentCount = 0, const glm::u8vec4 &color = {255, 255, 255, 255});

		void render_circle_frag(const glm::vec2 &position, f32 radius, const glm::u8vec4 &color = {255, 255, 255, 255});
	}// namespace Renderer2D
}// namespace fif::gfx
