#include "fif/gfx/renderer2d.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/rendererStats.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/renderable.h"
#include "fif/core/log.h"
#include "fif/core/transform.h"
#include "primitives/quad.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include <memory>

namespace fif::gfx {
	static RendererStats s_Stats, s_TempStats;
	static std::unique_ptr<Renderable> s_Quad, s_Circle;
	static std::unique_ptr<OrthoCamera> s_Camera;

	void Renderer2D::init() {
		s_Camera = std::make_unique<OrthoCamera>();
		s_Quad = std::make_unique<Renderable>(priv::primitives::Quad::VERTICES, 4, priv::primitives::Quad::ELEMENTS, 6, ShaderLibrary::get("quad"));
		s_Circle = std::make_unique<Renderable>(priv::primitives::Quad::VERTICES, 4, priv::primitives::Quad::ELEMENTS, 6, ShaderLibrary::get("circle"));
	}

	void Renderer2D::begin() {
		s_TempStats.primitivesRendered = 0u;
	}

	void Renderer2D::end() {
		s_Camera->update();
		s_Stats = s_TempStats;
	}

	static void render(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, const Renderable &renderable) {
		s_TempStats.primitivesRendered++;

		const glm::mat4 modelMatrix = core::Transform::getModelMatrix(glm::vec3(position, 0.0f), glm::vec3(size, 1.0f) * 0.5f, glm::vec3(0.0f));
		const glm::mat4 mvp = s_Camera->getMatrix() * modelMatrix;

		const Shader &shader = renderable.getShader(); 
		shader.bind();
		shader.setUniform("u_Color", color);
		shader.setUniform("u_MVP", mvp);
		renderable.render();
	}

	void Renderer2D::renderQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) {
		render(position, size, color, *s_Quad);
	}

	void Renderer2D::renderCircle(const glm::vec2 &position, float radius, const glm::vec4 &color) {
		render(position, {radius,radius}, color, *s_Circle);
	}

	const RendererStats &Renderer2D::getStats() {
		return s_Stats;
	}

	OrthoCamera &Renderer2D::getCamera() {
		return *s_Camera;
	}
}
