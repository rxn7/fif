#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Circle {
		const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		uniform mat4 u_ProjectionMatrix;

		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec2 a_UV;
		layout(location = 1) in vec4 a_Color;

		out vec4 v_Color;
		out vec2 v_UV;

		void main() {
			v_Color = a_Color;
			v_UV = a_UV;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;

		out vec4 f_Color;

		void main() {
			float dist = distance(vec2(0.5, 0.5), v_UV) * 2.0;
			float alpha = 1.0 - smoothstep(0.995, 1.0, clamp(dist, 0.0, 1.0));
			f_Color = vec4(v_Color.xyz, alpha * v_Color.a);
		})";
	}// namespace Circle
}// namespace fif::gfx::shaders
