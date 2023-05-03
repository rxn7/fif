#pragma once

#include "fif/core/opengl.hpp"
#include "fif/core/types.hpp"

#include "glm/ext/vector_uint2_sized.hpp"

#include <memory>

namespace fif::gfx {
	class Texture {
	public:
		Texture(u16 width, u16 height, GLenum internalFormat = GL_RGBA8, GLenum dataFormat = GL_RGBA, GLenum filter = GL_LINEAR, GLenum wrap = GL_CLAMP_TO_EDGE);
		Texture(const std::string &path, GLenum filter = GL_LINEAR, GLenum wrap = GL_CLAMP_TO_EDGE);
		~Texture();

		inline u32 getID() const { return m_ID; }
		inline u16 getWidth() const { return m_Width; }
		inline u16 getHeight() const { return m_Height; }
		inline glm::u16vec2 getSize() const { return glm::u16vec2(m_Width, m_Height); }

	private:
		void create(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap, void *data);

	public:
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
		u32 m_ID;
		u16 m_Width, m_Height;
	};
}// namespace fif::gfx
