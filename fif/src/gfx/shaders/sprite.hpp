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
		layout(location = 3) in int a_TextureSlot;
		layout(location = 4) in uint a_EntityID;

		out vec2 v_UV;
		out vec4 v_Color;
		flat out int v_TextureSlot;
		flat out uint v_EntityID;

		void main() {
			v_UV = a_UV;
			v_Color = a_Color;
			v_TextureSlot = a_TextureSlot;
			v_EntityID = a_EntityID;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;
		flat in int v_TextureSlot;
		flat in uint v_EntityID;

		layout (location = 0) out vec4 f_Color;
		layout (location = 1) out uint f_EntityID;

		uniform sampler2D u_Textures[$textureSlotCount];

		void main() {
			switch(v_TextureSlot)  {
				$textureSlotSwitch
			}
			f_Color *= v_Color;
			f_EntityID = v_EntityID;
		})";
	}// namespace Sprite
}// namespace fif::gfx::shaders
