#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct QuadVertex final {
		vec2 position;
		Color color;
	};

	inline const VertexBufferLayout QUAD_VERTEX_BUFFER_LAYOUT = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::UByte4, "a_Color", true},
	};
}// namespace fif::gfx
