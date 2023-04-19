#include "fif/gfx/shader.h"

#include <memory>

namespace fif::gfx::priv::shaders {
	namespace Circle {
		const std::string FRAGMENT = FIF_GLSL_VERSION FIF_GLSL_PRECISION
			R"(
		in vec4 v_Color;
		in vec2 v_UV;

		out vec4 f_Color;

		void main() {
			float dist = distance(vec2(0.5, 0.5), v_UV) * 2.0;
			float alpha = 1.0 - smoothstep(v_Color.a * 0.99, v_Color.a, dist);
			f_Color = vec4(v_Color.xyz, alpha);
		})";
	}
} // namespace fif::gfx::priv::shaders
