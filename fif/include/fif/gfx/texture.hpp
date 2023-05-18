#pragma once

#include "fif/core/opengl.hpp"

namespace fif::gfx {
	class Texture {
	public:
		Texture() {}
		Texture(u16 width, u16 height, GLenum internalFormat = GL_RGBA8, GLenum dataFormat = GL_RGBA, GLenum filter = GL_LINEAR, GLenum wrap = GL_CLAMP_TO_EDGE);
		Texture(std::string_view path, GLenum filter = GL_LINEAR, GLenum wrap = GL_CLAMP_TO_EDGE);
		~Texture();

		inline u32 get_id() const { return m_ID; }
		inline u16 get_width() const { return m_Width; }
		inline u16 get_height() const { return m_Height; }
		inline glm::u16vec2 get_size() const { return glm::u16vec2(m_Width, m_Height); }

		inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); }
		inline static void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

		inline bool operator==(const Texture &other) const { return other.get_id() == get_id(); }

		void bind_on_slot(u32 slot) const;
		void create(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap, void *data);

	public:
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
		u32 m_ID;
		u16 m_Width, m_Height;
	};
}// namespace fif::gfx
