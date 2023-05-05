#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct CircleVertex {
		glm::vec2 position;
		glm::vec2 uv;
		Color color;
	};

	inline const VertexBufferLayout circleVertexBufferLayout = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::Float2, "a_UV", false},
		{GlslDataType::UByte4, "a_Color", true},
	};
}// namespace fif::gfx
