#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Quad {
		const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		uniform mat4 u_ProjectionMatrix;

		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec4 a_Color;

		out vec4 v_Color;

		void main() {
			v_Color = a_Color;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		out vec4 f_Color;

		void main() {
			f_Color = v_Color;
		})";
	}// namespace Quad
}// namespace fif::gfx::shaders
