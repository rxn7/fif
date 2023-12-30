#pragma once

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/texture.hpp"
#include "shader.hpp"
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

		inline OrthoCamera &get_camera() {
			return m_Camera;
		}

		inline const Renderer2DStats &get_stats() const {
			return m_Stats;
		}

		template<typename Batch> void flush_batch(Batch &batch) {
			if(batch.is_empty())
				return;

			Shader &shader = batch.get_shader();

			shader.bind();
			shader.set_uniform_mat4("u_ProjectionMatrix", m_Camera.get_matrix());
			batch.flush();
			shader.unbind();

			m_TempStats.batchesFlushed++;
			m_TempStats.drawCallCount++;
		}

		void start();
		void end();

		void render_quad(const vec2 &position, const vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_sprite(const std::shared_ptr<Texture> &texture, const vec2 &position, const vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_circle(const vec2 &position, f32 radius, const Color &color = {255, 255, 255, 255});
		void render_text(const vec2 &position, f32 size, const std::string &text, const Color &color);
		f32 get_texture_slot(const std::shared_ptr<Texture> &texture);

	private:
		static constexpr u32 BATCH_SIZE = 1000;

		std::unique_ptr<Batch<QuadVertex>> mp_QuadBatch;
		std::unique_ptr<Batch<CircleVertex>> mp_CircleBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_SpriteBatch;

		OrthoCamera m_Camera;

		std::array<std::shared_ptr<Texture>, 32> m_Textures;
		i32 m_TextureIdx = 0;

		core::Clock::time_point m_BeginTime;
		Renderer2DStats m_Stats, m_TempStats;
		i32 m_TextureSlotCount;
	};
}// namespace fif::gfx
