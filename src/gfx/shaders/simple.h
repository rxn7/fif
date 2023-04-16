#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::priv::shaders {
	namespace Simple {
		const std::string VERTEX = 
		FIF_GLSL_VERSION
		FIF_GLSL_PRECISION
		R"(
		uniform mat4 u_MVP;

		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_UV;
		layout(location = 2) in vec4 a_Color;

		out vec2 v_UV;
		out vec4 v_Color;

		void main() {
			v_UV = a_UV;
			v_Color = a_Color;
			gl_Position = u_MVP * vec4(a_Position, 1.0);
		})";

		const std::string FRAGMENT =
		FIF_GLSL_VERSION
		FIF_GLSL_PRECISION
		R"(

		in vec4 v_Color;
		out vec4 f_Color;

		void main() {
			f_Color = v_Color;
		})";
	}
}
