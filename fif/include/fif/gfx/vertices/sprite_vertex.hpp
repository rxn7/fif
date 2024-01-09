#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct SpriteVertex final {
		vec2 position;
		vec2 uv;
		Color color;
		i32 textureSlot;
	};

	inline const VertexBufferLayout SPRITE_VERTEX_BUFFER_LAYOUT = {
		{GlslDataType::Float2, false, true},// a_Position
		{GlslDataType::Float2, false, true},// a_UV
		{GlslDataType::UByte4, true, true}, // a_Color
		{GlslDataType::Int, false, false},	// a_TextureSlot
	};
}// namespace fif::gfx
