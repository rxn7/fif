#include "fif/gfx/renderer2d.h"
#include "fif/gfx/batch.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/rendererStats.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/vertexBuffer.h"
#include "fif/core/log.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include <chrono>
#include <memory>
#include <ratio>

namespace fif::gfx {
	static constexpr std::uint32_t BATCH_SIZE = 100;

	static RendererStats s_Stats;

	static std::unique_ptr<Batch> s_SimpleBatch, s_CircleBatch;
	static std::unique_ptr<OrthoCamera> s_Camera;

	static std::chrono::time_point<std::chrono::high_resolution_clock> s_BeginTime;
	static std::uint32_t s_PrimitivesRenderered = 0u;

	void Renderer2D::init() {
		s_Camera = std::make_unique<OrthoCamera>();
		s_SimpleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
		s_CircleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
	}

	void Renderer2D::begin() {
		s_BeginTime = std::chrono::high_resolution_clock::now();
	}

	void Renderer2D::end() {
		s_Camera->update();

		{
			Shader &shader = ShaderLibrary::get("simple");
			shader.bind();
			shader.setUniform("u_MVP", s_Camera->getMatrix());
			s_SimpleBatch->render();
		}

		{
			Shader &shader = ShaderLibrary::get("circle");
			shader.bind();
			shader.setUniform("u_MVP", s_Camera->getMatrix());
			s_CircleBatch->render();
		}

		s_Stats.frameTimeMs = std::chrono::duration<float, std::milli>((std::chrono::high_resolution_clock::now() - s_BeginTime)).count();
		s_Stats.primitivesRendered = s_PrimitivesRenderered;

		s_PrimitivesRenderered = 0u;
	}

	void Renderer2D::renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle, const glm::u8vec4 &color) {
		const uint32_t vertCount = s_SimpleBatch->getVertexCount();

		glm::mat4 matrix(1.0f);
		matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
		matrix = glm::rotate(matrix, angle, {0,0,1});
		matrix = glm::scale(matrix, glm::vec3(size, 1.0));

		s_SimpleBatch->addVertex({glm::vec3(matrix * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f)), glm::vec2(0.0f,0.0f), color});
		s_SimpleBatch->addVertex({glm::vec3(matrix * glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)), glm::vec2(0.0f,1.0f), color});
		s_SimpleBatch->addVertex({glm::vec3(matrix * glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f)), glm::vec2(1.0f,1.0f), color});
		s_SimpleBatch->addVertex({glm::vec3(matrix * glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f)), glm::vec2(1.0f,0.0f), color});

		s_SimpleBatch->addElement(vertCount);
		s_SimpleBatch->addElement(vertCount+1);
		s_SimpleBatch->addElement(vertCount+2);
		s_SimpleBatch->addElement(vertCount+2);
		s_SimpleBatch->addElement(vertCount+3);
		s_SimpleBatch->addElement(vertCount);
	}

	void Renderer2D::renderCircleTriangle(const glm::vec2 &position, float diameter, std::uint16_t segments, const glm::u8vec4 &color) {
		const uint32_t vertCount = s_SimpleBatch->getVertexCount();

		const float segmentAngle = glm::two_pi<float>() / segments;
		const float radius = diameter * 0.5f;
		float angle = 0.0f;

		for(std::uint16_t i=0; i<segments; ++i) {
			s_SimpleBatch->addVertex({
				.position = glm::vec3(position, 0.0f) + glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0.0f),
				.color = color,
			});
			angle += segmentAngle;
		}

		for(std::uint16_t i=1; i<segments-1; ++i) {
			s_SimpleBatch->addElement(vertCount);
			s_SimpleBatch->addElement(vertCount+i);
			s_SimpleBatch->addElement(vertCount+i+1);
		}
	}

	void Renderer2D::renderCircleFrag(const glm::vec2 &position, float diameter, const glm::u8vec4 &color) {
		const uint32_t vertCount = s_CircleBatch->getVertexCount();
		const float radius = diameter * 0.5f;

		s_CircleBatch->addVertex({glm::vec3(position.x - radius, position.y - radius, 0.0f), glm::vec2(0.0f,0.0f), color});
		s_CircleBatch->addVertex({glm::vec3(position.x - radius, position.y + radius, 0.0f), glm::vec2(0.0f,1.0f), color});
		s_CircleBatch->addVertex({glm::vec3(position.x + radius, position.y + radius, 0.0f), glm::vec2(1.0f,1.0f), color});
		s_CircleBatch->addVertex({glm::vec3(position.x + radius, position.y - radius, 0.0f), glm::vec2(1.0f,0.0f), color});

		s_CircleBatch->addElement(vertCount);
		s_CircleBatch->addElement(vertCount+1);
		s_CircleBatch->addElement(vertCount+2);
		s_CircleBatch->addElement(vertCount+2);
		s_CircleBatch->addElement(vertCount+3);
		s_CircleBatch->addElement(vertCount);
	}

	const RendererStats &Renderer2D::getStats() {
		return s_Stats;
	}

	OrthoCamera &Renderer2D::getCamera() {
		return *s_Camera;
	}
}