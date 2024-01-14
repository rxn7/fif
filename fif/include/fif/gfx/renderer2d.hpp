#pragma once

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/render_command.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/resource/texture.hpp"
#include "fif/gfx/text/text_align.hpp"

#include "vertices/circle_vertex.hpp"
#include "vertices/quad_vertex.hpp"
#include "vertices/sprite_vertex.hpp"

#include <queue>
#include <regex>

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

		u32 glyphCount = 0;

		u32 textures = 0;
	};

	class Renderer2D final {
	public:
		Renderer2D();

		inline OrthoCamera &get_camera() { return m_Camera; }

		inline const Renderer2DStats &get_stats() const { return m_Stats; }

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

		template<typename VertexType> std::unique_ptr<Batch<VertexType>> setup_textured_batch(const VertexBufferLayout &layout, const std::string &vertexShader, const std::string &fragmentShader) {
			/*!
				NOTE: textureSlotSwitch is a goofy ahh hack to avoid this error: sampler arrays indexed with non-constant expressions are forbidden in
				GLSL 1.30 and later
			*/
			std::vector<i32> textureIndicesUniform(m_TextureSlotCount);
			std::ostringstream textureSlotSwitchContent;
			for(u16 i = 0; i < m_TextureSlotCount; ++i) {
				textureIndicesUniform[i] = i;
				textureSlotSwitchContent << "case " << i << ": f_Color = texture(u_Textures[" << i << "], v_UV); break;\n";
			}

			std::string fragModified = std::regex_replace(fragmentShader, std::regex("\\$textureSlotCount"), std::to_string(m_TextureSlotCount));
			fragModified = std::regex_replace(fragModified, std::regex("\\$textureSlotSwitch"), textureSlotSwitchContent.str());

			std::unique_ptr<Batch<VertexType>> batch = std::make_unique<Batch<VertexType>>(4, 6, BATCH_SIZE, layout, vertexShader, fragModified);
			batch->get_shader().bind();
			batch->get_shader().set_uniform_i32_array("u_Textures", textureIndicesUniform.data(), m_TextureSlotCount);
			batch->get_shader().unbind();

			return batch;
		}

		template<typename VertexType> u8 assign_texture_slot(const std::shared_ptr<Texture> &texture, Batch<VertexType> &batch) {
			for(i32 i = 0; i < m_TextureIdx; ++i) {
				if(m_Textures[i]->get_id() == texture->get_id())
					return i;
			}

			// TOOD: This probably won't work when multiple batches have
			// FLush the batch if it's full
			if(m_TextureIdx == m_TextureSlotCount) {
				flush_batch(batch);
				m_TextureIdx = 0;
			}

			const u8 textureSlot = m_TextureIdx;
			m_Textures[m_TextureIdx++] = texture;
			m_Textures[textureSlot]->bind_on_slot(textureSlot);

			return textureSlot;
		}

		void start();
		void end();

		template<class T, class... Args> inline void add_render_command(i8 zIndex, Args &&...args) {
			static_assert(std::is_base_of<RenderCommand, T>().value, "T must inherit RenderCommand");

			std::shared_ptr<RenderCommand> cmd = std::make_shared<T>(args...);
			cmd->zIndex = zIndex;

			m_RenderCommandsQueue.push(cmd);
		}

		void render_quad(const QuadRenderCommand &cmd);
		void render_sprite(const SpriteRenderCommand &cmd);
		void render_circle(const CircleRenderCommand &cmd);
		void render_text(const TextRenderCommand &cmd);

	private:
		void reset_textures();
		void execute_render_commands();
		void flush_all_batches();
		mat4 calculate_rotation_matrix(const vec2 &position, const vec2 &halfSize, const vec2 &pivot, const f32 angleRadians);

	private:
		// TODO: Find a middle ground for memory usage and performance.
		static constexpr u32 BATCH_SIZE = 1000;// In objects (4 vertices, 6 indices)

		std::unique_ptr<Batch<QuadVertex>> mp_QuadBatch;
		std::unique_ptr<Batch<CircleVertex>> mp_CircleBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_SpriteBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_GlyphBatch;

		std::priority_queue<std::shared_ptr<RenderCommand>, std::vector<std::shared_ptr<RenderCommand>>, RenderCommandComparator> m_RenderCommandsQueue;
		OrthoCamera m_Camera;

		std::array<std::shared_ptr<Texture>, 32> m_Textures;
		i32 m_TextureIdx = 0;

		core::Clock::time_point m_BeginTime;
		Renderer2DStats m_Stats, m_TempStats;
		i32 m_TextureSlotCount;
	};
}// namespace fif::gfx
