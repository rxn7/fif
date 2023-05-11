#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Sprite {
		const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		uniform mat4 u_ProjectionMatrix;

		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec2 a_UV;
		layout(location = 2) in vec4 a_Color;
		layout(location = 3) in float a_TextureSlot;

		out vec2 v_UV;
		out vec4 v_Color;
		out float v_TextureSlot;

		void main() {
			v_UV = a_UV;
			v_Color = a_Color;
			v_TextureSlot = a_TextureSlot;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;
		in float v_TextureSlot;

		out vec4 f_Color;

		uniform sampler2D u_Textures[$textureSlotCount];

		void main() {
			switch(int(v_TextureSlot))  {
				$generateTextureSlotSwitch
			}
			f_Color *= v_Color;
		})";
	}// namespace Sprite
}// namespace fif::gfx::shaders
