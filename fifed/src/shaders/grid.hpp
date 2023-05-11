#pragma once

#include "fif/gfx/shader.hpp"

namespace fifed::shaders::Grid {
	const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
		R"(
		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec2 a_UV;

		out vec2 v_UV;

		void main() {
			v_UV = a_UV;
			gl_Position = vec4(a_Position, 0.0, 1.0);
		})";

	const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
		// Yoinked from https://www.shadertoy.com/view/7tGBDK
		R"(
		uniform vec2 u_CameraSize;
		uniform vec2 u_CameraPosition;
		uniform vec3 u_LineColor;
		uniform vec3 u_BackgroundColor;
		uniform float u_LineThickness;
		uniform float u_MinCellSize;

		in vec2 v_UV;
		out vec4 f_Color;

		const float minCellPixelWidth = 2.0;

		const float log10Multiplier = 1.0 / log(10.0);

		float log10(float x) {
			return log(x) * log10Multiplier;
		}

		float max2(vec2 v) {
			return max(v.x, v.y);
		}

		vec4 grid(vec2 uv) {
			vec2 dudv = vec2(
				length(vec2(dFdx(uv.x), dFdy(uv.x))),
				length(vec2(dFdx(uv.y), dFdy(uv.y)))
			);

			float lod = max(0.0, log10((max2(dudv) * minCellPixelWidth) / u_MinCellSize) + 1.0);
			float fade = fract(lod);

			float lod0 = u_MinCellSize * pow(10.0, floor(lod));
			float lod1 = lod0 * 10.0;
			float lod2 = lod1 * 10.0;

			float lod0a = max2(vec2(1.0) - abs(clamp(mod(uv, lod0) / dudv / u_LineThickness, 0.0, 1.0) * 2.0 - vec2(1.0)));
			float lod1a = max2(vec2(1.0) - abs(clamp(mod(uv, lod1) / dudv / u_LineThickness, 0.0, 1.0) * 2.0 - vec2(1.0)));
			float lod2a = max2(vec2(1.0) - abs(clamp(mod(uv, lod2) / dudv / u_LineThickness, 0.0, 1.0) * 2.0 - vec2(1.0)));

			return vec4(
				lod2a > 0.0 ? u_LineColor : lod1a > 0.0 ? mix(u_LineColor, u_BackgroundColor, fade) : u_BackgroundColor,
				lod2a > 0.0 ? lod2a : lod1a > 0.0 ? lod1a : lod1a * (1.0 - fade)
			);
		}

		void main() {
			f_Color = grid(v_UV * u_CameraSize + u_CameraPosition);
		})";
}// namespace fifed::shaders::Grid
