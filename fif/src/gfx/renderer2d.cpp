#include "shaders/circle.hpp"
#include "shaders/glyph.hpp"
#include "shaders/quad.hpp"
#include "shaders/sprite.hpp"

#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/vertices/circle_vertex.hpp"
#include "fif/gfx/vertices/quad_vertex.hpp"

#include <regex>

#define FLUSH_IF_FULL(batch)                                                                                                                                                                           \
	if((batch)->is_full())                                                                                                                                                                             \
		flush_batch(*batch);

namespace fif::gfx {
	Renderer2D::Renderer2D() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);

		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_NEVER);

		glDisable(GL_DITHER);

		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_TextureSlotCount);
		core::Logger::info("[Renderer2D] Max texture slots: %d", m_TextureSlotCount);

		mp_QuadBatch = std::make_unique<Batch<QuadVertex>>(4, 6, BATCH_SIZE, QUAD_VERTEX_BUFFER_LAYOUT, shaders::Quad::VERTEX, shaders::Quad::FRAGMENT);
		mp_CircleBatch = std::make_unique<Batch<CircleVertex>>(4, 6, BATCH_SIZE, CIRCLE_VERTEX_BUFFER_LAYOUT, shaders::Circle::VERTEX, shaders::Circle::FRAGMENT);
		mp_GlyphBatch = setup_textured_batch<SpriteVertex>(SPRITE_VERTEX_BUFFER_LAYOUT, shaders::Sprite::VERTEX, shaders::Glyph::FRAGMENT);
		mp_SpriteBatch = setup_textured_batch<SpriteVertex>(SPRITE_VERTEX_BUFFER_LAYOUT, shaders::Sprite::VERTEX, shaders::Sprite::FRAGMENT);
	}

	void Renderer2D::start() {
		m_BeginTime = core::Clock::now();

		m_TextureIdx = 0;
		std::fill(m_Textures.begin(), m_Textures.end(), nullptr);
	}

	void Renderer2D::end() {
		m_Camera.update();

		flush_batch(*mp_QuadBatch);
		flush_batch(*mp_CircleBatch);
		flush_batch(*mp_SpriteBatch);
		flush_batch(*mp_GlyphBatch);

		m_Stats = m_TempStats;
		m_Stats.textures = m_TextureIdx;

		// Clean up
		m_TempStats = {0};
	}

	void Renderer2D::render_sprite(const std::shared_ptr<Texture> &texture, const vec2 &position, const vec2 &size, f32 angle, const Color &color, const std::array<vec2, 4> &uvs) {
		FLUSH_IF_FULL(mp_SpriteBatch)

		const vec2 halfSize = size * 0.5f;
		const u8 textureSlot = assign_texture_slot(texture, *mp_SpriteBatch);
		const u32 vertCount = mp_SpriteBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			m_TempStats.rotatedSpriteCount++;

			mat4 matrix(1.0f);
			matrix = translate(mat4(1.0f), vec3(position, 0.0f));
			matrix = rotate(matrix, -angle, {0, 0, 1});
			matrix = scale(matrix, vec3(halfSize, 1.0));

			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f)), uvs[0], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(-1.0f, 1.0f, 0.0f, 1.0f)), uvs[1], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(1.0f, 1.0f, 0.0f, 1.0f)), uvs[2], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(1.0f, -1.0f, 0.0f, 1.0f)), uvs[3], color, textureSlot});
		} else {
			m_TempStats.spriteCount++;
			mp_SpriteBatch->add_vertex({vec2(position.x - halfSize.x, position.y - halfSize.y), uvs[0], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x - halfSize.x, position.y + halfSize.y), uvs[1], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x + halfSize.x, position.y + halfSize.y), uvs[2], color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x + halfSize.x, position.y - halfSize.y), uvs[3], color, textureSlot});
		}

		mp_SpriteBatch->add_element(vertCount);
		mp_SpriteBatch->add_element(vertCount + 1);
		mp_SpriteBatch->add_element(vertCount + 2);
		mp_SpriteBatch->add_element(vertCount + 2);
		mp_SpriteBatch->add_element(vertCount + 3);
		mp_SpriteBatch->add_element(vertCount);

		m_TempStats.vertexCount += 4;
		m_TempStats.elementCount += 6;
	}

	void Renderer2D::render_quad(const vec2 &position, const vec2 &size, f32 angle, const Color &color) {
		FLUSH_IF_FULL(mp_QuadBatch)

		const vec2 halfSize = size * 0.5f;
		const u32 vertCount = mp_QuadBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			mat4 matrix(1.0f);
			matrix = translate(mat4(1.0f), vec3(position, 0.0f));
			matrix = rotate(matrix, -angle, {0, 0, 1});
			matrix = scale(matrix, vec3(halfSize, 1.0));
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(-1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(1.0f, -1.0f, 0.0f, 1.0f)), color});
			m_TempStats.rotatedQuadCount++;
		} else {
			mp_QuadBatch->add_vertex({vec2(position.x - halfSize.x, position.y - halfSize.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x - halfSize.x, position.y + halfSize.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x + halfSize.x, position.y + halfSize.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x + halfSize.x, position.y - halfSize.y), color});
			m_TempStats.quadCount++;
		}

		mp_QuadBatch->add_element(vertCount);
		mp_QuadBatch->add_element(vertCount + 1);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 3);
		mp_QuadBatch->add_element(vertCount);

		m_TempStats.vertexCount += 4;
		m_TempStats.elementCount += 6;
	}

	void Renderer2D::render_circle(const vec2 &position, f32 radius, const Color &color) {
		FLUSH_IF_FULL(mp_CircleBatch)

		const u32 vertCount = mp_CircleBatch->get_vertex_count();

		mp_CircleBatch->add_vertex({vec2(position.x - radius, position.y - radius), vec2(0.0f, 0.0f), color});
		mp_CircleBatch->add_vertex({vec2(position.x - radius, position.y + radius), vec2(0.0f, 1.0f), color});
		mp_CircleBatch->add_vertex({vec2(position.x + radius, position.y + radius), vec2(1.0f, 1.0f), color});
		mp_CircleBatch->add_vertex({vec2(position.x + radius, position.y - radius), vec2(1.0f, 0.0f), color});

		mp_CircleBatch->add_element(vertCount);
		mp_CircleBatch->add_element(vertCount + 1u);
		mp_CircleBatch->add_element(vertCount + 2u);
		mp_CircleBatch->add_element(vertCount + 2u);
		mp_CircleBatch->add_element(vertCount + 3u);
		mp_CircleBatch->add_element(vertCount);

		m_TempStats.circleCount++;
		m_TempStats.vertexCount += 4u;
		m_TempStats.elementCount += 6u;
	}

	// TODO: Support rotations
	void Renderer2D::render_text(const Font &font, const vec2 &position, const vec2 &scale, f32 fontSize, const std::string &text, const Color &color, const VerticalTextAlign vAlign, const HorizontalTextAlign hAlign) {
		FLUSH_IF_FULL(mp_GlyphBatch)

		const u8 textureSlot = assign_texture_slot(font.get_texture(), *mp_GlyphBatch);
		const vec2 textSize = font.calculate_text_size(text, scale * fontSize);
		const vec2 origin = position + TextAlign::get_text_align_offset(hAlign, vAlign, textSize, font.get_font_height() * fontSize);

		vec2 currentPosition = origin;

		for(std::string::const_iterator it = text.begin(); it < text.end(); ++it) {
			const bool isLastChar = it == text.end() - 1;

			switch(*it) {
			case '\\':
				if(isLastChar)
					break;

				switch(*(++it)) {
				case 'n':
					currentPosition.y -= font.get_font_height() * fontSize * scale.y;
					currentPosition.x = origin.x;
					break;
				case '\\':
					goto nobreak;
					break;
				}

				break;
			nobreak:

			default:
				if(*it < 32 || *it >= 127) {
					core::Logger::error("Char '%c' (%i) is out of supported range (32-127)", *it, static_cast<int>(*it));
					break;
				}

				const Glyph &glyph = font.get_glyph(*it);
				const vec2 glyphSize = static_cast<vec2>(glyph.size) * fontSize * scale;
				const u32 vertCount = mp_GlyphBatch->get_vertex_count();

				f32 x = currentPosition.x + glyph.offset.x * fontSize;
				f32 y = currentPosition.y - (glyph.size.y - glyph.offset.y) * fontSize;

				mp_GlyphBatch->add_vertex({vec2(x, y), {glyph.startUv.x, glyph.endUv.y}, color, textureSlot});
				mp_GlyphBatch->add_vertex({vec2(x, y + glyphSize.y), glyph.startUv, color, textureSlot});
				mp_GlyphBatch->add_vertex({vec2(x + glyphSize.x, y + glyphSize.y), {glyph.endUv.x, glyph.startUv.y}, color, textureSlot});
				mp_GlyphBatch->add_vertex({vec2(x + glyphSize.x, y), glyph.endUv, color, textureSlot});

				mp_GlyphBatch->add_element(vertCount);
				mp_GlyphBatch->add_element(vertCount + 1);
				mp_GlyphBatch->add_element(vertCount + 2);
				mp_GlyphBatch->add_element(vertCount + 2);
				mp_GlyphBatch->add_element(vertCount + 3);
				mp_GlyphBatch->add_element(vertCount);

				m_TempStats.vertexCount += 4;
				m_TempStats.elementCount += 6;
				m_TempStats.glyphCount++;

				currentPosition.x += glyph.advance.x * fontSize * scale.x;
			}
		}
	}
}// namespace fif::gfx
