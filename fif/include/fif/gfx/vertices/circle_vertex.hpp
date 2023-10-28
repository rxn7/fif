#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct CircleVertex final {
		vec2 position;
		vec2 uv;
		Color color;
	};

	inline const VertexBufferLayout CIRCLE_VERTEX_BUFFER_LAYOUT = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::Float2, "a_UV", false},
		{GlslDataType::UByte4, "a_Color", true},
	};
}// namespace fif::gfx
