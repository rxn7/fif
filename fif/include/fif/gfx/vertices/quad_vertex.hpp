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
		{GlslDataType::Float2, false, true},// a_Position
		{GlslDataType::UByte4, true, true}, // a_Color
	};
}// namespace fif::gfx
