#pragma once

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/shader_library.hpp"
#include "vertices/circle_vertex.hpp"
#include "vertices/quad_vertex.hpp"

namespace fif::gfx {
	struct Renderer2DStats {
		u32 circles = 0;
		u32 quads = 0;
		u32 vertices = 0;
		u32 elements = 0;
		u32 batchFlushes = 0;
	};

	class Renderer2D {
	public:
		Renderer2D();

		inline OrthoCamera &get_camera() const { return *mp_Camera; }
		inline const Renderer2DStats &get_stats() const { return m_Stats; }

		template<typename Vertex> void flush_batch(Batch<Vertex> &batch, const std::string &shaderName) {
			Shader &shader = ShaderLibrary::get(shaderName);
			shader.bind();
			shader.set_uniform("u_ProjectionMatrix", mp_Camera->get_matrix());
			batch.flush();
			m_TempStats.batchFlushes++;
		}

		void start();
		void end();

		void render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle = 0.0f, const glm::u8vec4 &color = {255, 255, 255, 255});
		void render_circle(const glm::vec2 &position, f32 radius, u16 segmentCount = 0, const glm::u8vec4 &color = {255, 255, 255, 255});
		void render_circle_frag(const glm::vec2 &position, f32 radius, const glm::u8vec4 &color = {255, 255, 255, 255});

	private:
		static constexpr u32 BATCH_SIZE = 10000;

		std::unique_ptr<Batch<QuadVertex>> mp_QuadBatch;
		std::unique_ptr<Batch<CircleVertex>> mp_CircleBatch;
		std::unique_ptr<OrthoCamera> mp_Camera;

		core::Clock::time_point m_BeginTime;
		Renderer2DStats m_Stats, m_TempStats;
		i32 m_TextureSlotCount;
	};
}// namespace fif::gfx
