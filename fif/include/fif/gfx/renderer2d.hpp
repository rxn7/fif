#pragma once

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/texture.hpp"
#include "vertices/circle_vertex.hpp"
#include "vertices/quad_vertex.hpp"
#include "vertices/sprite_vertex.hpp"

namespace fif::gfx {
	struct Renderer2DStats {
		u32 drawCallCount = 0;
		u32 batchesFlushed = 0;
		u32 vertexCount = 0;
		u32 elementCount = 0;

		u32 circleCount = 0;

		u32 quadCount = 0;
		u32 rotatedQuadCount = 0;

		u32 spriteCount = 0;
		u32 rotatedSpriteCount = 0;
	};

	class Renderer2D {
	public:
		Renderer2D();

		inline OrthoCamera &get_camera() const { return *mp_Camera; }
		inline const Renderer2DStats &get_stats() const { return m_Stats; }

		template<typename Vertex> void flush_batch(const std::unique_ptr<Batch<Vertex>> &batch) {
			if(batch->is_empty())
				return;

			Shader *shader = batch->get_shader();
			if(!shader)
				return;

			shader->bind();
			shader->set_uniform("u_ProjectionMatrix", mp_Camera->get_matrix());
			batch->flush();
			m_TempStats.batchesFlushed++;
			m_TempStats.drawCallCount++;
		}

		void start();
		void end();

		void render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_sprite(const std::shared_ptr<Texture> &texture, const glm::vec2 &position, const glm::vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_circle(const glm::vec2 &position, f32 radius, const Color &color = {255, 255, 255, 255});

	private:
		std::shared_ptr<Shader> create_sprite_shader();

	private:
		static constexpr u32 BATCH_SIZE = 1000;

		std::unique_ptr<Batch<QuadVertex>> mp_QuadBatch;
		std::unique_ptr<Batch<CircleVertex>> mp_CircleBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_SpriteBatch;
		std::unique_ptr<OrthoCamera> mp_Camera;

		std::array<std::shared_ptr<Texture>, 32> m_Textures;
		i32 m_TextureIdx = 0;

		core::Clock::time_point m_BeginTime;
		Renderer2DStats m_Stats, m_TempStats;
		i32 m_TextureSlotCount;
	};
}// namespace fif::gfx
