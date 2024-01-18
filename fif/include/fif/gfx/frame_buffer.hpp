#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/texture.hpp"

namespace fif::gfx {
	class FrameBuffer final {
	public:
		FrameBuffer(const vec2 &size);
		~FrameBuffer();

		inline u32 get_fbo_id() const { return m_FboID; }
		inline const std::shared_ptr<Texture> &get_texture() const { return mp_Texture; }
		inline const std::shared_ptr<Texture> &get_entity_id_texture() const { return mp_EntityIdTexture; }

		void start();
		void end();

		inline void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_FboID); }
		inline void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

		void invalidate();
		void set_size(const vec2 &size);
		inline const vec2 &get_size() const { return m_Size; }

		u32 read_entity_id_buffer_pixel(const u32vec2 &point) const;

	public:
		Color3 m_ClearColor = {200u, 200u, 200u};

	private:
		std::shared_ptr<Texture> mp_Texture;
		std::shared_ptr<Texture> mp_EntityIdTexture;
		vec2 m_Size;
		u32 m_FboID = 0;
	};
}// namespace fif::gfx
