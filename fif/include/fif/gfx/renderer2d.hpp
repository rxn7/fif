#pragma once

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/resource/texture.hpp"
#include "fif/gfx/text/text_align.hpp"

#include "vertices/circle_vertex.hpp"
#include "vertices/quad_vertex.hpp"
#include "vertices/sprite_vertex.hpp"

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

		template<typename VertexType> std::unique_ptr<Batch<VertexType>> setup_textured_batch(const VertexBufferLayout &layout, const std::string &vertexShader, const std::string &fragmentShader) {
			/*!
				NOTE: textureSlotSwitch is a goofy ahh hack to avoid this error: sampler arrays indexed with non-constant expressions are forbidden in
				GLSL 1.30 and later
			*/
			std::vector<i32> textureIndicesUniform(m_TextureSlotCount);
			std::ostringstream textureSlotSwitchContent;
			for(i32 i = 0; i < m_TextureSlotCount; ++i) {
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

		void start();
		void end();

		void start_ui();
		void end_ui();

		void render_quad(const vec2 &position, const vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_sprite(const std::shared_ptr<Texture> &texture, const vec2 &position, const vec2 &size, f32 angle = 0.0f, const Color &color = {255, 255, 255, 255});
		void render_circle(const vec2 &position, f32 radius, const Color &color = Colors::WHITE);
		void render_text(const Font &font, const vec2 &position, const vec2 &scale, f32 size, const std::string &text, const Color &color = Colors::BLACK, const VerticalTextAlign vAlign = VerticalTextAlign::Center, const HorizontalTextAlign hAlign = HorizontalTextAlign::Left);
		f32 assign_texture_slot(const std::shared_ptr<Texture> &texture);

	private:
		static constexpr u32 BATCH_SIZE = 1000;

		std::unique_ptr<Batch<QuadVertex>> mp_QuadBatch;
		std::unique_ptr<Batch<CircleVertex>> mp_CircleBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_SpriteBatch;
		std::unique_ptr<Batch<SpriteVertex>> mp_GlyphBatch;

		OrthoCamera m_Camera;
		OrthoCamera m_UICamera;

		std::array<std::shared_ptr<Texture>, 32> m_Textures;
		i32 m_TextureIdx = 0;

		core::Clock::time_point m_BeginTime;
		Renderer2DStats m_Stats, m_TempStats;
		i32 m_TextureSlotCount;
	};
}// namespace fif::gfx
