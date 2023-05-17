#include "fif/gfx/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"

namespace fif::gfx {
	Texture::Texture(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap) {
		glGenTextures(1, &m_ID);
		create(width, height, internalFormat, dataFormat, filter, wrap, nullptr);
	}

	Texture::~Texture() {
		glDeleteTextures(1, &m_ID);
	}

	std::shared_ptr<Texture> Texture::load(const std::string &path, GLenum filter, GLenum wrap) {
		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if(!data) {
			core::Logger::error("Failed to load texture: %s", path.c_str());
			return nullptr;
		}

		std::shared_ptr<Texture> texture(new Texture());
		glGenTextures(1, &texture->m_ID);

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

		texture->create(static_cast<u16>(width), static_cast<u16>(height), internalFormat, dataFormat, filter, wrap, data);

		stbi_image_free(data);

		return texture;
	}

	void Texture::create(u16 width, u16 height, GLenum internalFormat, GLenum dataFormat, GLenum filter, GLenum wrap, void *data) {
		m_Width = width;
		m_Height = height;
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

	void Texture::bind_on_slot(u32 slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}// namespace fif::gfx
