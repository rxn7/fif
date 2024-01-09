#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Glyph {
		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;
		flat in int v_TextureSlot;

		out vec4 f_Color;

		uniform sampler2D u_Textures[$textureSlotCount];

		void main() {
			switch(v_TextureSlot)  {
				$textureSlotSwitch
			}
			f_Color = f_Color.r * v_Color;
		})";
	}// namespace Glyph
}// namespace fif::gfx::shaders
