#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/vertices/circle_vertex.hpp"
#include "fif/gfx/vertices/quad_vertex.hpp"
#include "shaders/circle.hpp"
#include "shaders/quad.hpp"
#include "shaders/sprite.hpp"

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
		glDisable(GL_DITHER);

		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_TextureSlotCount);
		core::Logger::info("[Renderer2D] Max texture slots: %d", m_TextureSlotCount);

		mp_QuadBatch = std::make_unique<Batch<QuadVertex>>(4, 6, BATCH_SIZE, QUAD_VERTEX_BUFFER_LAYOUT, shaders::Quad::VERTEX, shaders::Quad::FRAGMENT);
		mp_CircleBatch = std::make_unique<Batch<CircleVertex>>(4, 6, BATCH_SIZE, CIRCLE_VERTEX_BUFFER_LAYOUT, shaders::Circle::VERTEX, shaders::Circle::FRAGMENT);

		{
			std::vector<i32> textureIndicesUniform(m_TextureSlotCount);
			/*!
				NOTE: textureSlotSwitch is a goofy ahh hack to avoid this error: sampler arrays indexed with non-constant expressions are forbidden in
				GLSL 1.30 and later
			*/
			std::ostringstream textureSlotSwitchContent;
			for(i32 i = 0; i < m_TextureSlotCount; ++i) {
				textureIndicesUniform[i] = i;
				textureSlotSwitchContent << "case " << i << ": f_Color = texture(u_Textures[" << i << "], v_UV); break;\n";
			}

			std::string spriteFrag = std::regex_replace(shaders::Sprite::FRAGMENT, std::regex("\\$textureSlotCount"), std::to_string(m_TextureSlotCount));
			spriteFrag = std::regex_replace(spriteFrag, std::regex("\\$textureSlotSwitch"), textureSlotSwitchContent.str());

			mp_SpriteBatch = std::make_unique<Batch<SpriteVertex>>(4, 6, BATCH_SIZE, SPRITE_VERTEX_BUFFER_LAYOUT, shaders::Sprite::VERTEX, spriteFrag);
			mp_SpriteBatch->get_shader().bind();
			mp_SpriteBatch->get_shader().set_uniform_i32_array("u_Textures", textureIndicesUniform.data(), m_TextureSlotCount);
			mp_SpriteBatch->get_shader().unbind();
		}
	}

	void Renderer2D::start() {
		m_Camera.update();
		m_BeginTime = core::Clock::now();
	}

	void Renderer2D::end() {
		flush_batch(*mp_QuadBatch);
		flush_batch(*mp_CircleBatch);
		flush_batch(*mp_SpriteBatch);

		m_Stats = m_TempStats;

		// Clean up
		m_TempStats = {0};
		m_TextureIdx = 0;
	}

	void Renderer2D::render_sprite(const std::shared_ptr<Texture> &texture, const vec2 &position, const vec2 &size, f32 angle, const Color &color) {
		FLUSH_IF_FULL(mp_SpriteBatch)

		f32 textureSlot = -1.0f;
		for(i32 i = 0; i < m_TextureIdx; ++i) {
			if(m_Textures[i]->get_id() == texture->get_id()) {
				textureSlot = static_cast<f32>(i);
				break;
			}
		}

		if(textureSlot == -1.0f) {
			if(m_TextureIdx == m_TextureSlotCount) {
				flush_batch(*mp_SpriteBatch);
				m_TextureIdx = 0;
			}

			textureSlot = static_cast<f32>(m_TextureIdx);
			m_Textures[m_TextureIdx++] = texture;
			m_Textures[textureSlot]->bind_on_slot(textureSlot);
		}

		const u32 vertCount = mp_SpriteBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			m_TempStats.rotatedSpriteCount++;

			mat4 matrix(1.0f);
			matrix = translate(mat4(1.0f), vec3(position, 0.0f));
			matrix = rotate(matrix, -angle, {0, 0, 1});
			matrix = scale(matrix, vec3(size, 1.0));

			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f)), vec2(0.0f, 0.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(-1.0f, 1.0f, 0.0f, 1.0f)), vec2(0.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(1.0f, 1.0f, 0.0f, 1.0f)), vec2(1.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec3(matrix * vec4(1.0f, -1.0f, 0.0f, 1.0f)), vec2(1.0f, 0.0f), color, textureSlot});
		} else {
			m_TempStats.spriteCount++;
			mp_SpriteBatch->add_vertex({vec2(position.x - size.x, position.y - size.y), vec2(0.0f, 0.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x - size.x, position.y + size.y), vec2(0.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x + size.x, position.y + size.y), vec2(1.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({vec2(position.x + size.x, position.y - size.y), vec2(1.0f, 0.0f), color, textureSlot});
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

		const u32 vertCount = mp_QuadBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			mat4 matrix(1.0f);
			matrix = translate(mat4(1.0f), vec3(position, 0.0f));
			matrix = rotate(matrix, -angle, {0, 0, 1});
			matrix = scale(matrix, vec3(size, 1.0));
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(-1.0f, -1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(-1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({vec2(matrix * vec4(1.0f, -1.0f, 0.0f, 1.0f)), color});
			m_TempStats.rotatedQuadCount++;
		} else {
			mp_QuadBatch->add_vertex({vec2(position.x - size.x, position.y - size.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x - size.x, position.y + size.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x + size.x, position.y + size.y), color});
			mp_QuadBatch->add_vertex({vec2(position.x + size.x, position.y - size.y), color});
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

	void Renderer2D::render_text(const vec2 &position, f32 size, const std::string &text, const Color &color) {
		FLUSH_IF_FULL(mp_QuadBatch)

		const u32 vertCount = mp_QuadBatch->get_vertex_count();

		mp_QuadBatch->add_vertex({vec2(position.x - size, position.y - size), color});
		mp_QuadBatch->add_vertex({vec2(position.x - size, position.y + size), color});
		mp_QuadBatch->add_vertex({vec2(position.x + size, position.y + size), color});
		mp_QuadBatch->add_vertex({vec2(position.x + size, position.y - size), color});
		m_TempStats.quadCount++;

		mp_QuadBatch->add_element(vertCount);
		mp_QuadBatch->add_element(vertCount + 1);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 2);
		mp_QuadBatch->add_element(vertCount + 3);
		mp_QuadBatch->add_element(vertCount);

		m_TempStats.vertexCount += 4;
		m_TempStats.elementCount += 6;
	}
}// namespace fif::gfx
