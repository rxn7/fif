#include "fif/gfx/renderer2d.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/log.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/shader_library.hpp"
#include "fif/gfx/vertex_buffer.hpp"

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "util/clock.hpp"

#include <chrono>
#include <memory>
#include <ratio>

namespace fif::gfx {
	static constexpr u32 BATCH_SIZE = 100000;

	static Renderer2D::Stats s_Stats, s_TempStats;
	static std::unique_ptr<Batch> s_SimpleBatch, s_CircleBatch;
	static std::unique_ptr<OrthoCamera> s_Camera;
	static core::Clock::time_point s_BeginTime;

	void Renderer2D::init() {
		s_Camera = std::make_unique<OrthoCamera>();
		s_SimpleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
		s_CircleBatch = std::make_unique<Batch>(4, 6, BATCH_SIZE);
	}

	void Renderer2D::begin() {
		s_BeginTime = core::Clock::now();
	}

	void Renderer2D::end() {
		s_Camera->update();

		{
			Shader &shader = ShaderLibrary::get("simple");
			shader.bind();
			shader.set_uniform("u_MVP", s_Camera->get_matrix());
			s_SimpleBatch->render();
		}

		{
			Shader &shader = ShaderLibrary::get("circle");
			shader.bind();
			shader.set_uniform("u_MVP", s_Camera->get_matrix());
			s_CircleBatch->render();
		}

		s_Stats = s_TempStats;
		s_TempStats = {};
	}

	void Renderer2D::render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle, const glm::u8vec4 &color) {
		if(!s_Camera->contains_quad(position, size))
			return;

		const u32 vertCount = s_SimpleBatch->get_vertex_count();
		glm::mat4 matrix(1.0F);
		matrix = glm::translate(glm::mat4(1.0F), glm::vec3(position, 0.0F));
		matrix = glm::rotate(matrix, angle, {0, 0, 1});
		matrix = glm::scale(matrix, glm::vec3(size, 1.0));

		s_SimpleBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 0.0F), color});
		s_SimpleBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 1.0F), color});
		s_SimpleBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 1.0F), color});
		s_SimpleBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 0.0F), color});

		s_SimpleBatch->add_element(vertCount);
		s_SimpleBatch->add_element(vertCount + 1);
		s_SimpleBatch->add_element(vertCount + 2);
		s_SimpleBatch->add_element(vertCount + 2);
		s_SimpleBatch->add_element(vertCount + 3);
		s_SimpleBatch->add_element(vertCount);

		s_TempStats.quads++;
		s_TempStats.vertices += 4;
		s_TempStats.elements += 6;
	}

	void Renderer2D::render_circle(const glm::vec2 &position, f32 radius, u16 segmentCount, const glm::u8vec4 &color) {
		FIF_ASSERT(segmentCount > 2u, "Circle must have at least 3 segments!");

		if(!s_Camera->contains_circle(position, radius))
			return;

		const u32 vertCount = s_SimpleBatch->get_vertex_count();
		const f32 segmentAngle = glm::two_pi<f32>() / segmentCount;
		f32 angle = 0.0F;

		for(u16 i = 0; i < segmentCount; ++i) {
			s_SimpleBatch->add_vertex({
				.position = position + glm::vec2(radius * glm::cos(angle), radius * glm::sin(angle)),
				.color = color,
			});
			angle += segmentAngle;
		}

		for(u16 i = 1u; i < segmentCount - 1u; ++i) {
			s_SimpleBatch->add_element(vertCount);
			s_SimpleBatch->add_element(vertCount + i);
			s_SimpleBatch->add_element(vertCount + i + 1u);
		}

		s_TempStats.circles++;
		s_TempStats.vertices += vertCount;
		s_TempStats.elements += (segmentCount - 2u) * 3u;
	}

	void Renderer2D::render_circle_frag(const glm::vec2 &position, f32 radius, const glm::u8vec4 &color) {
		if(!s_Camera->contains_circle(position, radius))
			return;

		const u32 vertCount = s_CircleBatch->get_vertex_count();

		s_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y - radius), glm::vec2(0.0F, 0.0F), color});
		s_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y + radius), glm::vec2(0.0F, 1.0F), color});
		s_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y + radius), glm::vec2(1.0F, 1.0F), color});
		s_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y - radius), glm::vec2(1.0F, 0.0F), color});

		s_CircleBatch->add_element(vertCount);
		s_CircleBatch->add_element(vertCount + 1u);
		s_CircleBatch->add_element(vertCount + 2u);
		s_CircleBatch->add_element(vertCount + 2u);
		s_CircleBatch->add_element(vertCount + 3u);
		s_CircleBatch->add_element(vertCount);

		s_TempStats.circles++;
		s_TempStats.vertices += 4u;
		s_TempStats.elements += 6u;
	}

	Renderer2D::Stats &Renderer2D::get_stats() {
		return s_Stats;
	}

	OrthoCamera &Renderer2D::get_camera() {
		return *s_Camera;
	}
}// namespace fif::gfx
