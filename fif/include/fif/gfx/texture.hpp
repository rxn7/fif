#pragma once

#include "fif/core/opengl.hpp"
#include "fif/core/resource.hpp"

namespace fif::gfx {
	class Texture final : public core::Resource {
	public:
		Texture();

		Texture(u16 width, u16 height, const GLenum internalFormat = GL_RGBA8, const GLenum dataFormat = GL_RGBA, const GLenum filter = GL_LINEAR, const GLenum wrap = GL_CLAMP_TO_EDGE, void *data = nullptr);
		Texture(const bool isEditorResource, const std::filesystem::path &path, const GLenum filter = GL_LINEAR, const GLenum wrap = GL_CLAMP_TO_EDGE);

		~Texture();

		inline u32 get_id() const {
			return m_ID;
		}
		inline u16 get_width() const {
			return m_Width;
		}
		inline u16 get_height() const {
			return m_Height;
		}
		inline u16vec2 get_size() const {
			return u16vec2(m_Width, m_Height);
		}

		inline void bind() const {
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}
		inline static void unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline bool operator==(const Texture &other) const {
			return other.get_id() == get_id();
		}

		void bind_on_slot(const u32 slot) const;
		void create(const u16 width, const u16 height, const GLenum internalFormat, const GLenum dataFormat, const GLenum filter, const GLenum wrap, void *data);

	public:
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
		u32 m_ID;
		u16 m_Width, m_Height;
	};
}// namespace fif::gfx
