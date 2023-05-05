#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/shader_library.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/vertices/circle_vertex.hpp"
#include "fif/gfx/vertices/quad_vertex.hpp"

namespace fif::gfx {
	Renderer2D::Renderer2D() {
		FIF_LOG("[Renderer2D] OpenGL Renderer: " << glGetString(GL_RENDERER));
		FIF_LOG("[Renderer2D] OpenGL Version: " << glGetString(GL_VERSION));
		FIF_LOG("[Renderer2D] OpenGL Vendor: " << glGetString(GL_VENDOR));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		i32 textureSlotCount = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureSlotCount);

		FIF_LOG("[Renderer2D] Max texture slots: " << textureSlotCount);

		m_TextureSlotCount = textureSlotCount;
		mp_Camera = std::make_unique<OrthoCamera>();
		mp_QuadBatch = std::make_unique<Batch<QuadVertex>>(4, 6, BATCH_SIZE, quadVertexBufferLayout);
		mp_CircleBatch = std::make_unique<Batch<CircleVertex>>(4, 6, BATCH_SIZE, circleVertexBufferLayout);
	}

	void Renderer2D::start() {
		m_BeginTime = core::Clock::now();
	}

	void Renderer2D::end() {
		mp_Camera->update();

		flush_batch(*mp_QuadBatch, "quad");
		flush_batch(*mp_CircleBatch, "circle");

		m_Stats = m_TempStats;
		m_TempStats = {0};
	}

	void Renderer2D::render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle, const glm::u8vec4 &color) {
		if(!mp_Camera->contains_quad(position, size))
			return;

		const u32 vertCount = mp_QuadBatch->get_vertex_count();
		glm::mat4 matrix(1.0F);
		matrix = glm::translate(glm::mat4(1.0F), glm::vec3(position, 0.0F));
		matrix = glm::rotate(matrix, angle, {0, 0, 1});
		matrix = glm::scale(matrix, glm::vec3(size, 1.0));

		mp_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 0.0F), color});
		mp_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(-0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(0.0F, 1.0F), color});
		mp_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, 0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 1.0F), color});
		mp_QuadBatch->add_vertex({glm::vec3(matrix * glm::vec4(0.5F, -0.5F, 0.0F, 1.0F)), glm::vec2(1.0F, 0.0F), color});

		mp_QuadBatch->add_element(vertCount);
		mp_QuadBatch->add_element(vertCount + 1);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 3);
		mp_QuadBatch->add_element(vertCount);

		m_TempStats.quads++;
		m_TempStats.vertices += 4;
		m_TempStats.elements += 6;
	}

	void Renderer2D::render_circle(const glm::vec2 &position, f32 radius, u16 segmentCount, const glm::u8vec4 &color) {
		FIF_ASSERT(segmentCount > 2u, "Circle must have at least 3 segments!");

		if(!mp_Camera->contains_circle(position, radius))
			return;

		const u32 vertCount = mp_QuadBatch->get_vertex_count();
		const f32 segmentAngle = glm::two_pi<f32>() / segmentCount;
		f32 angle = 0.0F;

		for(u16 i = 0; i < segmentCount; ++i) {
			mp_QuadBatch->add_vertex({
				.position = position + glm::vec2(radius * glm::cos(angle), radius * glm::sin(angle)),
				.uv = {0, 0},
				.color = color,
			});
			angle += segmentAngle;
		}

		for(u16 i = 1u; i < segmentCount - 1u; ++i) {
			mp_QuadBatch->add_element(vertCount);
			mp_QuadBatch->add_element(vertCount + i);
			mp_QuadBatch->add_element(vertCount + i + 1u);
		}

		m_TempStats.circles++;
		m_TempStats.vertices += vertCount;
		m_TempStats.elements += (segmentCount - 2u) * 3u;
	}

	void Renderer2D::render_circle_frag(const glm::vec2 &position, f32 radius, const glm::u8vec4 &color) {
		if(!mp_Camera->contains_circle(position, radius))
			return;

		const u32 vertCount = mp_CircleBatch->get_vertex_count();

		mp_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y - radius), glm::vec2(0.0F, 0.0F), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y + radius), glm::vec2(0.0F, 1.0F), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y + radius), glm::vec2(1.0F, 1.0F), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y - radius), glm::vec2(1.0F, 0.0F), color});

		mp_CircleBatch->add_element(vertCount);
		mp_CircleBatch->add_element(vertCount + 1u);
		mp_CircleBatch->add_element(vertCount + 2u);
		mp_CircleBatch->add_element(vertCount + 2u);
		mp_CircleBatch->add_element(vertCount + 3u);
		mp_CircleBatch->add_element(vertCount);

		m_TempStats.circles++;
		m_TempStats.vertices += 4u;
		m_TempStats.elements += 6u;
	}
}// namespace fif::gfx
