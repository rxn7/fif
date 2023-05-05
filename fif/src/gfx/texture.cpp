#include "fif/gfx/texture.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/util/log.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"

namespace fif::gfx {
	Texture::Texture(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap) {
		glGenTextures(1, &m_ID);

		create(width, height, internalFormat, dataFormat, filter, wrap, nullptr);
	}

	Texture::Texture(const std::string &path, GLenum filter, GLenum wrap) {
		glGenTextures(1, &m_ID);

		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if(!data) {
			FIF_LOG_ERROR("Failed to load texture: " << path);
			return;
		}

		GLenum internalFormat, dataFormat;

		switch(channels) {
		default:
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		}

		create(static_cast<u16>(width), static_cast<u16>(height), internalFormat, dataFormat, filter, wrap, data);

		stbi_image_free(data);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_ID);
	}

	void Texture::create(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap, void *data) {
		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}// namespace fif::gfx
