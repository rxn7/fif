#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/shader_library.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/vertices/circle_vertex.hpp"
#include "fif/gfx/vertices/quad_vertex.hpp"

namespace fif::gfx {
	static constexpr u32 BATCH_SIZE = 100000;

	static Renderer2D::Stats s_Stats, s_TempStats;

	static std::unique_ptr<Batch<QuadVertex>> s_QuadBatch;
	static std::unique_ptr<Batch<CircleVertex>> s_CircleBatch;

	static std::unique_ptr<OrthoCamera> s_Camera;
	static core::Clock::time_point s_BeginTime;
	static i32 s_TextureSlotCount;

	void Renderer2D::init(i32 textureSlotCount) {
		s_TextureSlotCount = textureSlotCount;
		s_Camera = std::make_unique<OrthoCamera>();
		s_QuadBatch = std::make_unique<Batch<QuadVertex>>(4, 6, BATCH_SIZE, quadVertexBufferLayout);
		s_CircleBatch = std::make_unique<Batch<CircleVertex>>(4, 6, BATCH_SIZE, circleVertexBufferLayout);
	}

	void Renderer2D::begin() {
		s_BeginTime = core::Clock::now();
	}

	void Renderer2D::end() {
		s_Camera->update();

		{
			Shader &shader = ShaderLibrary::get("quad");
			shader.bind();
			shader.set_uniform("u_ProjectionMatrix", s_Camera->get_matrix());
			s_QuadBatch->flush();
		}

		{
			Shader &shader = ShaderLibrary::get("circle");
			shader.bind();
			shader.set_uniform("u_ProjectionMatrix", s_Camera->get_matrix());
			s_CircleBatch->flush();
		}

		s_Stats = s_TempStats;
		s_TempStats = {};
	}

	void Renderer2D::render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle, const glm::u8vec4 &color) {
		if(!s_Camera->contains_quad(position, size))
			return;

		const u32 vertCount = s_QuadBatch->get_vertex_count();
		glm::mat4 matrix(1.0F);
		matrix = glm::translate(glm::mat4(1.0F), glm::vec3(position, 0.0F));
		matrix = glm::rotate(matrix, angle, {0, 0, 1});
		matrix = glm::scale(matrix, glm::vec3(size, 1.0));

		s_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 0.0F), color});
		s_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 1.0F), color});
		s_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 1.0F), color});
		s_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 0.0F), color});

		s_QuadBatch->add_element(vertCount);
		s_QuadBatch->add_element(vertCount + 1);
		s_QuadBatch->add_element(vertCount + 2);
		s_QuadBatch->add_element(vertCount + 2);
		s_QuadBatch->add_element(vertCount + 3);
		s_QuadBatch->add_element(vertCount);

		s_TempStats.quads++;
		s_TempStats.vertices += 4;
		s_TempStats.elements += 6;
	}

	void Renderer2D::render_circle(const glm::vec2 &position, f32 radius, u16 segmentCount, const glm::u8vec4 &color) {
		FIF_ASSERT(segmentCount > 2u, "Circle must have at least 3 segments!");

		if(!s_Camera->contains_circle(position, radius))
			return;

		const u32 vertCount = s_QuadBatch->get_vertex_count();
		const f32 segmentAngle = glm::two_pi<f32>() / segmentCount;
		f32 angle = 0.0F;

		for(u16 i = 0; i < segmentCount; ++i) {
			s_QuadBatch->add_vertex({
				.position = position + glm::vec2(radius * glm::cos(angle), radius * glm::sin(angle)),
				.uv = {0, 0},
				.color = color,
			});
			angle += segmentAngle;
		}

		for(u16 i = 1u; i < segmentCount - 1u; ++i) {
			s_QuadBatch->add_element(vertCount);
			s_QuadBatch->add_element(vertCount + i);
			s_QuadBatch->add_element(vertCount + i + 1u);
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
