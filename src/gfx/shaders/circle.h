#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::priv::shaders {
	namespace Circle {
		const std::string VERTEX = 
		Shader::GLSL_VERSION + 
		R"(
		layout(location = 0) in vec3 l_Position;
		layout(location = 1) in vec2 l_UV;

		out vec2 v_UV;

		void main() {
			v_UV = l_UV;
			gl_Position = vec4(l_Position, 1.0);
		})";

		const std::string FRAGMENT =
		Shader::GLSL_VERSION + 
		R"(
		uniform vec4 u_Color;
		in vec2 v_UV;

		out vec4 f_Color;

		void main() {
			float dist = distance(vec2(0.5, 0.5), v_UV) * 2.0;
			float alpha = dist > 0.99 ? 1.0 - smoothstep(0.99, 1.0, dist) : 1.0;
			f_Color = mix(vec4(0.0), u_Color, alpha);
		})";
	}
}
