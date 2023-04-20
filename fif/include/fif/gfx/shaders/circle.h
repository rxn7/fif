#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::shaders {
	namespace Circle {
		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;

		out vec4 f_Color;

		void main() {
			float dist = distance(vec2(0.5, 0.5), v_UV) * 2.0;
			float alpha = 1.0 - smoothstep(0.99, 1.0, clamp(dist, 0.0, 1.0));
			f_Color = vec4(v_Color.xyz, alpha * v_Color.a);
		})";
	}
} // namespace fif::gfx::shaders
