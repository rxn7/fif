#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	struct SpriteVertex final {
		glm::vec2 position;
		glm::vec2 uv;
		Color color;
		f32 textureSlot;
	};

	inline const VertexBufferLayout spriteVertexBufferLayout = {
		{GlslDataType::Float2, "a_Position", false},
		{GlslDataType::Float2, "a_UV", false},
		{GlslDataType::UByte4, "a_Color", true},
		{GlslDataType::Float, "a_TextureSlot", false},
	};
}// namespace fif::gfx