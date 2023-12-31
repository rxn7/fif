#pragma once

#include "fif/gfx/text/glyph.hpp"
#include "fif/gfx/texture.hpp"

#include <array>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fif::gfx {
	class Font {// TODO: Extend resource ?
	public:
		Font(const std::string &path, const u32 size = 16u, const GLint filter = GL_LINEAR);
		virtual ~Font();

		vec2 calculate_text_size(const std::string &text, const f32 size) const;

		inline const std::shared_ptr<Texture> &get_texture() const {
			return mp_Texture;
		}

		inline const Glyph &get_glyph(char c) const {
			return m_Glyphs.at(c);
		}

		inline f32 get_font_height() const {
			return m_FontHeight;
		}

		inline u32 get_font_size() const {
			return m_Size;
		}

	private:
		FT_Face m_Face;

		std::shared_ptr<Texture> mp_Texture;
		std::unordered_map<char, Glyph> m_Glyphs;

		u32 m_Size;
		f32 m_FontHeight = 0;
	};
}// namespace fif::gfx
