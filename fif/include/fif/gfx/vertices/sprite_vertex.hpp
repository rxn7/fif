#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct SpriteVertex final {
		vec2 position;
		vec2 uv;
		Color color;
		f32 textureSlot;
	};

	inline const VertexBufferLayout SPRITE_VERTEX_BUFFER_LAYOUT = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::Float2, "a_UV", false},
		{GlslDataType::UByte4, "a_Color", true},
		{GlslDataType::Float, "a_TextureSlot", false},
	};
}// namespace fif::gfx
