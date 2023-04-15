#include "fif/gfx/renderer2d.h"
#include "fif/gfx/rendererStats.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/renderable.h"
#include "fif/core/log.h"
#include "primitives/quad.h"

#include <memory>

namespace fif::gfx {
	static RendererStats s_Stats, s_TempStats;
	static std::unique_ptr<Renderable> s_Quad, s_Circle;

	void Renderer2D::init() {
		s_Quad = std::make_unique<Renderable>(priv::primitives::Quad::VERTICES, priv::primitives::Quad::ELEMENTS, ShaderLibrary::get("quad"));
		s_Circle = std::make_unique<Renderable>(priv::primitives::Quad::VERTICES, priv::primitives::Quad::ELEMENTS, ShaderLibrary::get("circle"));
	}

	void Renderer2D::begin() {
		s_TempStats.primitivesRendered = 0u;
	}

	void Renderer2D::end() {
		s_Stats = s_TempStats;
	}

	const RendererStats &getStats() {
		return s_Stats;
	}

	void Renderer2D::renderQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
	}

	void Renderer2D::renderCircle(const glm::vec2 &position, float radiu, const glm::vec4 &color) {
		s_TempStats.primitivesRendered++;

		const Shader &shader = s_Circle->getShader(); 
		shader.bind();
		shader.setUniform("u_Color", color);
		s_Circle->render();
	}
}
