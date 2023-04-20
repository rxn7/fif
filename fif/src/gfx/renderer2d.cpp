#include "fif/gfx/renderer2d.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/log.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/orthoCamera.hpp"
#include "fif/gfx/shaderLibrary.hpp"
#include "fif/gfx/vertexBuffer.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <chrono>
#include <memory>
#include <ratio>

namespace fif::gfx {
	static constexpr u32 BATCH_SIZE = 100000;

	static Renderer2D::Stats s_Stats, s_TempStats;
	static std::unique_ptr<Batch> s_SimpleBatch, s_CircleBatch;
	static std::unique_ptr<OrthoCamera> s_Camera;
	static std::chrono::time_point<std::chrono::high_resolution_clock> s_BeginTime;

	void Renderer2D::init() {
		s_Camera = std::make_unique<OrthoCamera>();
		s_SimpleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
		s_CircleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
	}

	void Renderer2D::begin() {
		FIF_PROFILE_FUNC();
		s_BeginTime = std::chrono::high_resolution_clock::now();
	}

	void Renderer2D::end() {
		FIF_PROFILE_FUNC();
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

		s_Stats = s_TempStats;
		s_TempStats = {};
	}

	void Renderer2D::renderQuad(const glm::vec2 &position, const glm::vec2 &size, float angle,
								const glm::u8vec4 &color) {
		FIF_PROFILE_FUNC();

		if (!s_Camera->containsQuad(position, size)) {
			return;
		}

		const u32 vertCount = s_SimpleBatch->getVertexCount();
		glm::mat4 matrix(1.0F);
		matrix = glm::translate(glm::mat4(1.0F), glm::vec3(position, 0.0F));
		matrix = glm::rotate(matrix, angle, {0, 0, 1});
		matrix = glm::scale(matrix, glm::vec3(size, 1.0));

		s_SimpleBatch->addVertex({glm::vec3(matrix * glm::vec4(-0.5F, -0.5F, 0.0F, 1.0F)),
								  glm::vec2(0.0F, 0.0F), color});
		s_SimpleBatch->addVertex(
			{glm::vec3(matrix * glm::vec4(-0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 1.0F), color});
		s_SimpleBatch->addVertex(
			{glm::vec3(matrix * glm::vec4(0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 1.0F), color});
		s_SimpleBatch->addVertex(
			{glm::vec3(matrix * glm::vec4(0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 0.0F), color});

		s_SimpleBatch->addElement(vertCount);
		s_SimpleBatch->addElement(vertCount + 1);
		s_SimpleBatch->addElement(vertCount + 2);
		s_SimpleBatch->addElement(vertCount + 2);
		s_SimpleBatch->addElement(vertCount + 3);
		s_SimpleBatch->addElement(vertCount);

		s_TempStats.quads++;
		s_TempStats.vertices += 4;
		s_TempStats.elements += 6;
	}

	void Renderer2D::renderCircleTriangle(const glm::vec2 &position, float radius, u16 segmentCount,
										  const glm::u8vec4 &color) {
		FIF_PROFILE_FUNC();
		FIF_ASSERT(segmentCount > 2, "Circle must have at least 3 segments!");

		if (!s_Camera->containsCircle(position, radius)) {
			return;
		}

		const u32 vertCount = s_SimpleBatch->getVertexCount();
		const float segmentAngle = glm::two_pi<float>() / segmentCount;
		float angle = 0.0F;

		for (u16 i = 0; i < segmentCount; ++i) {
			s_SimpleBatch->addVertex({
				.position =
					position + glm::vec2(radius * glm::cos(angle), radius * glm::sin(angle)),
				.color = color,
			});
			angle += segmentAngle;
		}

		for (u16 i = 1; i < segmentCount - 1; ++i) {
			s_SimpleBatch->addElement(vertCount);
			s_SimpleBatch->addElement(vertCount + i);
			s_SimpleBatch->addElement(vertCount + i + 1);
		}

		s_TempStats.circles++;
		s_TempStats.vertices += vertCount;
		s_TempStats.elements += (segmentCount - 2) * 3;
	}

	void Renderer2D::renderCircleFrag(const glm::vec2 &position, float radius,
									  const glm::u8vec4 &color) {
		FIF_PROFILE_FUNC();

		if (!s_Camera->containsCircle(position, radius)) {
			return;
		}

		const u32 vertCount = s_CircleBatch->getVertexCount();

		s_CircleBatch->addVertex(
			{glm::vec2(position.x - radius, position.y - radius), glm::vec2(0.0F, 0.0F), color});
		s_CircleBatch->addVertex(
			{glm::vec2(position.x - radius, position.y + radius), glm::vec2(0.0F, 1.0F), color});
		s_CircleBatch->addVertex(
			{glm::vec2(position.x + radius, position.y + radius), glm::vec2(1.0F, 1.0F), color});
		s_CircleBatch->addVertex(
			{glm::vec2(position.x + radius, position.y - radius), glm::vec2(1.0F, 0.0F), color});

		s_CircleBatch->addElement(vertCount);
		s_CircleBatch->addElement(vertCount + 1);
		s_CircleBatch->addElement(vertCount + 2);
		s_CircleBatch->addElement(vertCount + 2);
		s_CircleBatch->addElement(vertCount + 3);
		s_CircleBatch->addElement(vertCount);

		s_TempStats.circles++;
		s_TempStats.vertices += 4;
		s_TempStats.elements += 6;
	}

	Renderer2D::Stats &Renderer2D::getStats() {
		return s_Stats;
	}

	OrthoCamera &Renderer2D::getCamera() {
		return *s_Camera;
	}
} // namespace fif::gfx
