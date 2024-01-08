#include "fif/gfx/resource/texture.hpp"
#include "fif/core/project.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include <stb_image.h>

namespace fif::gfx {
	Texture::Texture(const u16 width, const u16 height, const GLenum internalFormat, const GLenum dataFormat, const GLenum filter, const GLenum wrap, void *data) {
		glGenTextures(1, &m_ID);
		create(width, height, internalFormat, dataFormat, filter, wrap, data);
	}

	Texture::Texture(const std::filesystem::path &path, GLenum filter, GLenum wrap) : core::Resource(path) {
		i32 width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc *data = stbi_load(get_path_relative().string().c_str(), &width, &height, &channels, 0);

		if(!data) {
			core::Logger::error("Failed to load texture '%s': %s", get_path_relative().string().c_str(), stbi_failure_reason());
			return;
		}

		glGenTextures(1, &m_ID);

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

	Texture::~Texture() { glDeleteTextures(1, &m_ID); }

	void Texture::create(const u16 width, const u16 height, const GLenum internalFormat, const GLenum dataFormat, const GLenum filter, const GLenum wrap, void *data) {
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

	void Texture::bind_on_slot(const u32 slot) const {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}// namespace fif::gfx
