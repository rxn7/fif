#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct CircleVertex final {
		vec2 position;
		vec2 uv;
		Color color;
		u32 entityID;// TODO: Use this only in editor?
	};

	inline const VertexBufferLayout CIRCLE_VERTEX_BUFFER_LAYOUT = {
		{GlslDataType::Float2, false, true},// a_Position
		{GlslDataType::Float2, false, true},// a_UV
		{GlslDataType::UByte4, true, true}, // a_Color
		{GlslDataType::UInt, false, false}, // a_EntityID
	};
}// namespace fif::gfx
