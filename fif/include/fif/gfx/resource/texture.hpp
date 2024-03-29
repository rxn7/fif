#pragma once

#include "fif/core/opengl.hpp"
#include "fif/core/resource/resource.hpp"

namespace fif::gfx {
	class Texture final : public core::Resource {
	public:
		FIF_RESOURCE("Texture");
		Texture();

		Texture(const u16vec2 &size, const GLenum internalFormat = GL_RGBA8, const GLenum dataFormat = GL_RGBA, const GLenum filter = GL_LINEAR, const GLenum wrap = GL_CLAMP_TO_EDGE, void *data = nullptr);
		Texture(const std::filesystem::path &path, const bool isEditorResource = true, GLenum filter = GL_LINEAR, GLenum wrap = GL_CLAMP_TO_EDGE);

		~Texture();

		inline u32 get_id() const { return m_ID; }
		inline u16 get_width() const { return m_Size.x; }
		inline u16 get_height() const { return m_Size.y; }
		inline u16vec2 get_size() const { return m_Size; }

		inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_ID); }
		inline static void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

		inline bool operator==(const Texture &other) const { return other.get_id() == get_id(); }

		void bind_on_slot(const u16 slot) const;
		void create(const u16vec2 &size, const GLenum internalFormat, const GLenum dataFormat, const GLenum filter, const GLenum wrap, void *data);
		void read_pixel(const u32 x, const u32 y) const;

	private:
		GLenum m_InternalFormat;
		GLenum m_DataFormat;
		u32 m_ID;
		u16vec2 m_Size;
	};
}// namespace fif::gfx
