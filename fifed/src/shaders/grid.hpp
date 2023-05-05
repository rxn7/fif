#pragma once

#include "fif/gfx/shader.hpp"

namespace fifed::shaders::Grid {
	const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
		R"(
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_UV;

		out vec2 v_UV;

		void main() {
			v_UV = a_UV;
			gl_Position = vec4(a_Position, 1.0);
		})";

	// TODO: Fix aliasing

	const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
		R"(
		uniform vec2 u_CameraSize;
		uniform vec2 u_CameraPosition;
		uniform vec4 u_LineColor;
		uniform float u_LineThickness;
		uniform float u_CellSize;

		in vec2 v_UV;
		out vec4 f_Color;

		void main() {
			vec2 p = mod(v_UV * u_CameraSize + u_CameraPosition + u_LineThickness * 0.5, u_CellSize);

			float x = step(u_LineThickness, p.x);
			float y = step(u_LineThickness, p.y);

			float grid = min(x, y);

			f_Color = mix(u_LineColor, vec4(0.0), grid);
		})";
}// namespace fifed::shaders::Grid
