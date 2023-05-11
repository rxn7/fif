#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fifed {
	namespace Grid {
		void init();
		void render();

		inline bool enabled = true;
		inline Color3 lineColor = Color3(20, 20, 20);
		inline Color3 backgroundColor = Color3(200, 200, 200);
		inline f32 lineThickness = 5.0f;
		inline f32 minCellSize = 0.05f;

		struct GridVertex {
			glm::vec2 position;
			glm::vec2 uv;
		};

		inline const VertexBufferLayout gridVertexBufferLayout = {
			{GlslDataType::Float2, "a_Position", false},
			{GlslDataType::Float2, "a_UV", false},
		};
	};// namespace Grid
}// namespace fifed
