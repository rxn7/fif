#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::priv::shaders {
	namespace Quad {
		const std::string VERTEX = 
		Shader::GLSL_VERSION + 
		R"(
		uniform mat4 u_MVP;

		layout(location = 0) in vec3 l_Position;
		layout(location = 1) in vec2 l_UV;

		out vec2 v_UV;

		void main() {
			v_UV = l_UV;
			gl_Position = u_MVP * vec4(l_Position, 1.0);
		})";

		const std::string FRAGMENT =
		Shader::GLSL_VERSION + 
		R"(
		uniform vec4 u_Color;

		out vec4 f_Color;

		void main() {
			f_Color = u_Color;
		})";
	}
}
