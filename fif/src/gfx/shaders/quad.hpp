#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx::shaders {
	namespace Quad {
		const std::string VERTEX = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		uniform mat4 u_ProjectionMatrix;

		layout(location = 0) in vec2 a_Position;
		layout(location = 1) in vec4 a_Color;
		)"
#if FIF_MOUSE_PICKING
			"layout(location = 2) in uint a_EntityID;"
#endif

			R"(
		out vec4 v_Color;
		)"

#if FIF_MOUSE_PICKING
			"flat out uint v_EntityID;"
#endif

			R"(
		void
			main() {
			v_Color = a_Color;
			v_EntityID = a_EntityID;
			gl_Position = u_ProjectionMatrix * vec4(a_Position, 0.0, 1.0);
		})";

		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		flat in uint v_EntityID;

		layout (location = 0) out vec4 f_Color;
		layout (location = 1) out uint f_EntityID;

		void main() {
			f_Color = v_Color;
			f_EntityID = v_EntityID;
		})";
	}// namespace Quad
}// namespace fif::gfx::shaders
