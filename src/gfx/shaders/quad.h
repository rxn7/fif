#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::priv::shaders {
	namespace Quad {
		const std::string VERTEX = R"(
		#version 330 core

		layout(location = 0) in vec3 l_Position;
		layout(location = 1) in vec2 l_UV;
		layout(location = 2) in vec4 l_Color;

		out vec4 v_Color;
		out vec2 v_UV;

		void main() {
			v_Color = l_Color;
			v_UV = l_UV;
			gl_Position = vec4(l_Position, 1.0);
		})";

		const std::string FRAGMENT = R"(
		#version 330 core

		in vec4 v_Color;
		in vec2 v_UV;

		out vec4 f_Color;

		void main() {
			f_Color = v_Color * vec4(0.0, v_UV, 1.0);
		})";
	}
}
