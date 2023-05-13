#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/batch.hpp"
#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/shader_library.hpp"
#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/vertices/circle_vertex.hpp"
#include "fif/gfx/vertices/quad_vertex.hpp"
#include "shaders/circle.hpp"
#include "shaders/quad.hpp"
#include "shaders/sprite.hpp"

#include <regex>

#define FLUSH_IF_FULL(batch, shader)                                                                                                                 \
	if((batch)->is_full())                                                                                                                           \
		flush_batch(*(batch), *(shader));

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

		mp_Camera = std::make_unique<OrthoCamera>();
		mp_QuadBatch = std::make_unique<Batch<QuadVertex>>(4, 6, BATCH_SIZE, quadVertexBufferLayout);
		mp_CircleBatch = std::make_unique<Batch<CircleVertex>>(4, 6, BATCH_SIZE, circleVertexBufferLayout);
		mp_SpriteBatch = std::make_unique<Batch<SpriteVertex>>(4, 6, BATCH_SIZE, spriteVertexBufferLayout);

		// NOTE: textureSlotSwitch is a goofy ahh hack to avoid this error: sampler arrays indexed with non-constant expressions are forbidden in
		// GLSL 1.30 and later
		std::vector<i32> texturesUniform(m_TextureSlotCount);
		std::ostringstream textureSlotSwitchContent;
		for(i32 i = 0; i < m_TextureSlotCount; ++i) {
			texturesUniform[i] = i;
			textureSlotSwitchContent << "case " << i << ": f_Color = texture(u_Textures[" << i << "], v_UV); break;\n";
		}

		std::string spriteFrag = std::regex_replace(shaders::Sprite::FRAGMENT, std::regex("\\$textureSlotCount"), std::to_string(m_TextureSlotCount));
		spriteFrag = std::regex_replace(spriteFrag, std::regex("\\$textureSlotSwitch"), textureSlotSwitchContent.str());

		mp_SpriteShader = ShaderLibrary::add("sprite", shaders::Sprite::VERTEX, spriteFrag);
		mp_CircleShader = ShaderLibrary::add("circle", shaders::Circle::VERTEX, shaders::Circle::FRAGMENT);
		mp_QuadShader = ShaderLibrary::add("quad", shaders::Quad::VERTEX, shaders::Quad::FRAGMENT);

		mp_SpriteShader->bind();
		mp_SpriteShader->set_uniform("u_Textures", texturesUniform.data(), m_TextureSlotCount);
	}

	void Renderer2D::start() {
		mp_Camera->update();
		m_BeginTime = core::Clock::now();
	}

	void Renderer2D::end() {
		flush_batch(*mp_QuadBatch, *mp_QuadShader);
		flush_batch(*mp_CircleBatch, *mp_CircleShader);
		flush_batch(*mp_SpriteBatch, *mp_SpriteShader);

		m_Stats = m_TempStats;

		// Clean up
		m_TempStats = {0};
		m_Textures.fill(nullptr);
		m_TextureIdx = 0;
	}

	void Renderer2D::render_sprite(const std::shared_ptr<Texture> &texture, const glm::vec2 &position, const glm::vec2 &size, f32 angle, const Color &color) {
		FLUSH_IF_FULL(mp_SpriteBatch, mp_SpriteShader)

		f32 textureSlot = -1.0f;
		for(i32 i = 0; i < m_TextureIdx; ++i) {
			if(m_Textures[i]->get_id() == texture->get_id()) {
				textureSlot = static_cast<f32>(i);
				break;
			}
		}

		if(textureSlot == -1.0f) {
			if(m_TextureIdx == m_TextureSlotCount) {
				flush_batch(*mp_SpriteBatch, *mp_SpriteShader);
				m_TextureIdx = 0;
			}

			textureSlot = static_cast<f32>(m_TextureIdx);
			m_Textures[m_TextureIdx++] = texture;
			m_Textures[textureSlot]->bind_on_slot(textureSlot);
		}

		const u32 vertCount = mp_SpriteBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			m_TempStats.rotatedSpriteCount++;

			glm::mat4 matrix(1.0f);
			matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
			matrix = glm::rotate(matrix, -angle, {0, 0, 1});
			matrix = glm::scale(matrix, glm::vec3(size, 1.0));

			mp_SpriteBatch->add_vertex({glm::vec3(matrix * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f)), glm::vec2(0.0f, 0.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec3(matrix * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f)), glm::vec2(0.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec3(matrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)), glm::vec2(1.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec3(matrix * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)), glm::vec2(1.0f, 0.0f), color, textureSlot});
		} else {
			m_TempStats.spriteCount++;
			mp_SpriteBatch->add_vertex({glm::vec2(position.x - size.x, position.y - size.y), glm::vec2(0.0f, 0.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec2(position.x - size.x, position.y + size.y), glm::vec2(0.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec2(position.x + size.x, position.y + size.y), glm::vec2(1.0f, 1.0f), color, textureSlot});
			mp_SpriteBatch->add_vertex({glm::vec2(position.x + size.x, position.y - size.y), glm::vec2(1.0f, 0.0f), color, textureSlot});
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

	void Renderer2D::render_quad(const glm::vec2 &position, const glm::vec2 &size, f32 angle, const Color &color) {
		FLUSH_IF_FULL(mp_QuadBatch, mp_QuadShader)

		const u32 vertCount = mp_QuadBatch->get_vertex_count();

		if(glm::mod(angle, glm::two_pi<f32>())) {
			glm::mat4 matrix(1.0f);
			matrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
			matrix = glm::rotate(matrix, -angle, {0, 0, 1});
			matrix = glm::scale(matrix, glm::vec3(size, 1.0));
			mp_QuadBatch->add_vertex({glm::vec2(matrix * glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({glm::vec2(matrix * glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({glm::vec2(matrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)), color});
			mp_QuadBatch->add_vertex({glm::vec2(matrix * glm::vec4(1.0f, -1.0f, 0.0f, 1.0f)), color});
			m_TempStats.rotatedQuadCount++;
		} else {
			mp_QuadBatch->add_vertex({glm::vec2(position.x - size.x, position.y - size.y), color});
			mp_QuadBatch->add_vertex({glm::vec2(position.x - size.x, position.y + size.y), color});
			mp_QuadBatch->add_vertex({glm::vec2(position.x + size.x, position.y + size.y), color});
			mp_QuadBatch->add_vertex({glm::vec2(position.x + size.x, position.y - size.y), color});
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

	void Renderer2D::render_circle(const glm::vec2 &position, f32 radius, const Color &color) {
		FLUSH_IF_FULL(mp_CircleBatch, mp_CircleShader)

		const u32 vertCount = mp_CircleBatch->get_vertex_count();

		mp_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y - radius), glm::vec2(0.0f, 0.0f), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x - radius, position.y + radius), glm::vec2(0.0f, 1.0f), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y + radius), glm::vec2(1.0f, 1.0f), color});
		mp_CircleBatch->add_vertex({glm::vec2(position.x + radius, position.y - radius), glm::vec2(1.0f, 0.0f), color});

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
}// namespace fif::gfx
