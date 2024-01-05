#pragma once

#include "fif/gfx/text/glyph.hpp"
#include "fif/gfx/texture.hpp"

#include <array>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fif::gfx {
	class Font final : public core::Resource {
	public:
		Font(const bool isEditorResource, const std::filesystem::path &path, const u32 size = 16u, const u32 textureSize = 512, const GLenum filter = GL_LINEAR);
		~Font();

		vec2 calculate_text_size(const std::string &text, const vec2 &size) const;

		inline const std::shared_ptr<Texture> &get_texture() const {
			return mp_Texture;
		}

		inline const Glyph &get_glyph(char c) const {
			const auto it = m_Glyphs.find(c);

			if(it != m_Glyphs.end())
				return it->second;

			// Return any glyph
			return m_Glyphs.begin()->second;
		}

		inline f32 get_font_height() const {
			return m_FontHeight;
		}

		inline u32 get_font_size() const {
			return m_Size;
		}

		static inline const std::shared_ptr<Font> &get_default() {
			return sp_DefaultFont;
		}

	private:
		inline static std::shared_ptr<Font> sp_DefaultFont;
		FT_Face m_Face;

		std::shared_ptr<Texture> mp_Texture;
		std::unordered_map<char, Glyph> m_Glyphs;

		u32 m_Size;
		f32 m_FontHeight = 0;

		friend class GfxModule;
	};
}// namespace fif::gfx
