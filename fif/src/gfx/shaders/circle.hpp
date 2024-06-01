#pragma once

#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Circle {
		const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		uniform mat4 u_ProjectionMatrix;

		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec2 a_UV;
		layout(location = 2) in vec4 a_Color;
		layout(location = 3) in uint a_EntityID;

		out vec4 v_Color;
		out vec2 v_UV;
		flat out uint v_EntityID;

		void main() {
			v_Color = a_Color;
			v_UV = a_UV;
			v_EntityID = a_EntityID;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
									 R"(
		in vec4 v_Color;
		in vec2 v_UV;
		flat in uint v_EntityID;

		layout (location = 0) out vec4 f_Color;
		layout (location = 1) out uint f_EntityID;

		void main() {
			float dist = clamp(distance(vec2(0.5, 0.5), v_UV) * 2.0, 0.0, 1.0);

			 // TODO: Avoid the if statement
			if(dist > 0.5) {
				f_EntityID = )" + std::to_string(Renderer2D::INVALID_ENTITY_ID) +
									 R"(u;
				discard;
			}

			float alpha = 1.0 - smoothstep(0.995, 1.0, dist);
			f_EntityID = v_EntityID;
			f_Color = vec4(v_Color.xyz, alpha * v_Color.a);
		})";
	}// namespace Circle
}// namespace fif::gfx::shaders
