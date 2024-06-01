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

#ifdef FIF_MOUSE_PICKING
#define ENTITY_ID_IF_MOUSE_PICKING_ENABLED , cmd.entityID
#else
#define ENTITY_ID_IF_MOUSE_PICKING_ENABLED
#endif

#define FLUSH_IF_FULL(batch)                                                                                                                                                                           \
	if((batch)->is_full())                                                                                                                                                                             \
		flush_batch(*batch);

namespace fif::gfx {
	Renderer2D::Renderer2D() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// NOTE: Disabled, culling in 2D isn't needed
		// glEnable(GL_CULL_FACE);
		// glFrontFace(GL_CW);
		// glCullFace(GL_BACK);

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

	void Renderer2D::start() { reset_textures(); }

	void Renderer2D::end() {
		m_Camera.update();

		m_TempStats.renderCommands = m_RenderCommandsQueue.size();

		execute_render_commands();
		flush_all_batches();

		m_Stats = m_TempStats;
		m_Stats.textures = m_TextureIdx;

		m_TempStats = {0};
	}

	void Renderer2D::reset_textures() {
		m_TextureIdx = 0;
		std::fill(m_Textures.begin(), m_Textures.end(), nullptr);
	}

	void Renderer2D::execute_render_commands() {
		i8 lastZIndex = std::numeric_limits<i8>::min();
		while(!m_RenderCommandsQueue.empty()) {
			std::shared_ptr<RenderCommand> cmd = m_RenderCommandsQueue.top();
			m_RenderCommandsQueue.pop();

			cmd->render(*this);

			if(cmd->zIndex != lastZIndex) {
				flush_all_batches();
			}

			lastZIndex = cmd->zIndex;
		}
	}

	void Renderer2D::flush_all_batches() {
		flush_batch(*mp_QuadBatch);
		flush_batch(*mp_CircleBatch);
		flush_batch(*mp_GlyphBatch);
		flush_batch(*mp_SpriteBatch);
	}

	void Renderer2D::render_sprite(const SpriteRenderCommand &cmd) {
		FLUSH_IF_FULL(mp_SpriteBatch)

		const vec2 halfSize = cmd.size * 0.5f;
		const u8 textureSlot = assign_texture_slot(cmd.p_Texture, *mp_SpriteBatch);
		const u32 vertCount = mp_SpriteBatch->get_vertex_count();

		const u16vec2 uvSize = cmd.uvSize == u16vec2(0, 0) ? u16vec2(cmd.p_Texture->get_size()) : cmd.uvSize;

		const vec2 uvStart = vec2(cmd.uvPivot) / vec2(cmd.p_Texture->get_size());
		const vec2 uvEnd = vec2(cmd.uvPivot + uvSize) / vec2(cmd.p_Texture->get_size());
		const bool isRotated = glm::mod(cmd.angle, glm::two_pi<f32>());

		if(isRotated) {
			m_TempStats.rotatedSpriteCount++;
			const mat4 matrix = calculate_rotation_matrix(cmd.position, halfSize, cmd.pivot, cmd.angle);
			mp_SpriteBatch->add_vertex({matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f), uvStart, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({matrix * vec4(-1.0f, +1.0f, 0.0f, 1.0f), {uvStart.x, uvEnd.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({matrix * vec4(+1.0f, +1.0f, 0.0f, 1.0f), uvEnd, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({matrix * vec4(+1.0f, -1.0f, 0.0f, 1.0f), {uvEnd.x, uvStart.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
		} else {
			m_TempStats.spriteCount++;
			mp_SpriteBatch->add_vertex({{cmd.position.x - halfSize.x, cmd.position.y - halfSize.y}, uvStart, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({{cmd.position.x - halfSize.x, cmd.position.y + halfSize.y}, {uvStart.x, uvEnd.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({{cmd.position.x + halfSize.x, cmd.position.y + halfSize.y}, {uvEnd.x, uvEnd.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_SpriteBatch->add_vertex({{cmd.position.x + halfSize.x, cmd.position.y - halfSize.y}, {uvEnd.x, uvStart.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
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

	void Renderer2D::render_quad(const QuadRenderCommand &cmd) {
		FLUSH_IF_FULL(mp_QuadBatch)

		const vec2 halfSize = cmd.size * 0.5f;
		const u32 vertCount = mp_QuadBatch->get_vertex_count();
		const bool isRotated = glm::mod(cmd.angle, glm::two_pi<f32>());

		if(isRotated) {
			const mat4 matrix = calculate_rotation_matrix(cmd.position, halfSize, cmd.pivot, cmd.angle);
			mp_QuadBatch->add_vertex({matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({matrix * vec4(-1.0f, +1.0f, 0.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({matrix * vec4(+1.0f, +1.0f, 0.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({matrix * vec4(+1.0f, -1.0f, 0.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			m_TempStats.rotatedQuadCount++;
		} else {
			mp_QuadBatch->add_vertex({{cmd.position.x - halfSize.x, cmd.position.y - halfSize.y}, cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({{cmd.position.x - halfSize.x, cmd.position.y + halfSize.y}, cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({{cmd.position.x + halfSize.x, cmd.position.y + halfSize.y}, cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
			mp_QuadBatch->add_vertex({{cmd.position.x + halfSize.x, cmd.position.y - halfSize.y}, cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
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

	void Renderer2D::render_circle(const CircleRenderCommand &cmd) {
		FLUSH_IF_FULL(mp_CircleBatch)
		const u32 vertCount = mp_CircleBatch->get_vertex_count();

		mp_CircleBatch->add_vertex({{cmd.position.x - cmd.radius, cmd.position.y - cmd.radius}, vec2(0.0f, 0.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
		mp_CircleBatch->add_vertex({{cmd.position.x - cmd.radius, cmd.position.y + cmd.radius}, vec2(0.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
		mp_CircleBatch->add_vertex({{cmd.position.x + cmd.radius, cmd.position.y + cmd.radius}, vec2(1.0f, 1.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
		mp_CircleBatch->add_vertex({{cmd.position.x + cmd.radius, cmd.position.y - cmd.radius}, vec2(1.0f, 0.0f), cmd.color ENTITY_ID_IF_MOUSE_PICKING_ENABLED});

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
	void Renderer2D::render_text(const TextRenderCommand &cmd) {
		FLUSH_IF_FULL(mp_GlyphBatch)

		const u8 textureSlot = assign_texture_slot(cmd.font.get_texture(), *mp_GlyphBatch);
		const vec2 textSize = cmd.font.calculate_text_size(cmd.text, cmd.size);
		const vec2 origin = cmd.position + TextAlign::get_text_align_offset(cmd.hAlign, cmd.vAlign, textSize, cmd.font.get_font_height() * cmd.size.y);
		const bool isRotated = glm::mod(cmd.angle, glm::two_pi<f32>());

		vec2 currentPosition = origin;

		const auto break_line = [&currentPosition, &origin, &cmd]() {
			currentPosition.y -= cmd.font.get_font_height() * cmd.size.y;
			currentPosition.x = origin.x;
		};

		for(std::string::const_iterator it = cmd.text.begin(); it < cmd.text.end(); ++it) {
			const bool isLastChar = it == cmd.text.end() - 1;

			switch(*it) {
			case '\n':
				break_line();
				break;

			case '\\':
				if(isLastChar)
					break;

				switch(*(++it)) {
				case 'n':
					break_line();
					break;
				case '\\':
					goto nobreak;
					break;
				}

				break;
			nobreak:

			default:
				if(*it < 32 || *it >= 127) {
					core::Logger::error("Char '%c' (%i) is out of supported range (32-127)", *it, static_cast<int>(*it));
					break;
				}

				const u32 vertCount = mp_GlyphBatch->get_vertex_count();

				const Glyph &glyph = cmd.font.get_glyph(*it);
				const vec2 glyphSize = static_cast<vec2>(glyph.size) * cmd.size;
				const vec2 glyphPosition = vec2(currentPosition.x + glyph.offset.x * cmd.size.x, currentPosition.y - (glyph.size.y - glyph.offset.y) * cmd.size.x);

				// TODO: Rotated text rendering
				if(isRotated) {
					m_TempStats.rotatedGlyphCount++;
				} else {
					m_TempStats.glyphCount++;
				}

				mp_GlyphBatch->add_vertex({glyphPosition, {glyph.uvStart.x, glyph.uvEnd.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
				mp_GlyphBatch->add_vertex({{glyphPosition.x, glyphPosition.y + glyphSize.y}, glyph.uvStart, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
				mp_GlyphBatch->add_vertex({{glyphPosition.x + glyphSize.x, glyphPosition.y + glyphSize.y}, {glyph.uvEnd.x, glyph.uvStart.y}, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});
				mp_GlyphBatch->add_vertex({{glyphPosition.x + glyphSize.x, glyphPosition.y}, glyph.uvEnd, cmd.color, textureSlot ENTITY_ID_IF_MOUSE_PICKING_ENABLED});

				mp_GlyphBatch->add_element(vertCount);
				mp_GlyphBatch->add_element(vertCount + 1);
				mp_GlyphBatch->add_element(vertCount + 2);
				mp_GlyphBatch->add_element(vertCount + 2);
				mp_GlyphBatch->add_element(vertCount + 3);
				mp_GlyphBatch->add_element(vertCount);

				m_TempStats.vertexCount += 4;
				m_TempStats.elementCount += 6;

				currentPosition.x += glyph.advance.x * cmd.size.x;
			}
		}
	}

	mat4 Renderer2D::calculate_rotation_matrix(const vec2 &position, const vec2 &halfSize, const vec2 &pivot, const f32 angleRadians) {
		const glm::mat4 translateToPivot = glm::translate(mat4(1.0f), vec3(-pivot, 0.0f));
		const glm::mat4 translateFromPivot = glm::translate(mat4(1.0f), vec3(pivot, 0.0f));
		const glm::mat4 rotate = translateToPivot * glm::rotate(mat4(1.0f), -angleRadians, {0, 0, 1}) * translateFromPivot;

		mat4 matrix = glm::translate(mat4(1.0f), vec3(position + pivot, 0.0f));
		matrix *= rotate;
		matrix = glm::scale(matrix, vec3(halfSize, 1.0));

		return matrix;
	}
}// namespace fif::gfx
