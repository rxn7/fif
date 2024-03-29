#include "fif/gfx/resource/font.hpp"
#include "fif/gfx/gfx_module.hpp"

#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fif::gfx {
	Font::Font(const std::filesystem::path &path, const bool isEditorResource, const u32 size, const u32 textureSize, const GLenum filter) : core::Resource(path, isEditorResource) {
		if(FT_Error error = FT_New_Face(GfxModule::get_instance().m_FreeType, get_path_relative().string().c_str(), 0, &m_Face)) {
			core::Logger::error("Failed to load freetype font: %s", FT_Error_String(error));
			return;
		}

		FT_Set_Pixel_Sizes(m_Face, 0, size);
		constexpr u32 glyphPadding = 2u;
		u32 row = 0;
		u32 col = glyphPadding;

		u8 *textureBuffer = (u8 *)alloca(textureSize * textureSize * sizeof(u8));
		std::fill_n(textureBuffer, textureSize * textureSize, 0);

		for(char c = 32; c < 127; ++c) {
			const u32 glyphIdx = FT_Get_Char_Index(m_Face, c);

			FT_Load_Glyph(m_Face, glyphIdx, FT_LOAD_DEFAULT);
			FT_Error error = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
			FIF_ASSERT(error == 0, "Failed to render glyph");

			// Move the glyph to the next row if it won't fix on this row.
			if(col + m_Face->glyph->bitmap.width + glyphPadding >= textureSize) {
				col = glyphPadding;
				row += size + glyphPadding;
			}

			m_FontHeight = std::max(m_FontHeight, static_cast<f32>((m_Face->size->metrics.ascender - m_Face->size->metrics.descender) >> 6));

			if((row + m_Face->glyph->bitmap.rows) * textureSize + col + m_Face->glyph->bitmap.width >= textureSize * textureSize) {
				core::Logger::error("Glyph won't find into the font's texture!\nTexture size: %u\nGlyph: '%c'", textureSize, c);
				break;
			}

			for(u32 y = 0; y < m_Face->glyph->bitmap.rows; ++y) {
				for(u32 x = 0; x < m_Face->glyph->bitmap.width; ++x) {
					textureBuffer[(row + y) * textureSize + col + x] = m_Face->glyph->bitmap.buffer[y * m_Face->glyph->bitmap.width + x];
				}
			}

			Glyph glyph;
			glyph.uvStart = {static_cast<f32>(col) / textureSize, static_cast<f32>(row) / textureSize};
			glyph.uvEnd = {static_cast<f32>(col + m_Face->glyph->bitmap.width) / textureSize, static_cast<f32>(row + m_Face->glyph->bitmap.rows) / textureSize};
			glyph.size = {m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows};
			glyph.advance = {m_Face->glyph->advance.x >> 6, m_Face->glyph->advance.y >> 6};
			glyph.offset = {m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top};
			m_Glyphs.insert({c, glyph});

			col += m_Face->glyph->bitmap.width + glyphPadding;
		}

		FT_Done_Face(m_Face);

		mp_Texture = std::make_shared<Texture>(u16vec2(textureSize, textureSize), GL_R8, GL_RED, filter, GL_CLAMP_TO_BORDER, textureBuffer);
	}

	Font::~Font() {}

	// TODO: Width should be calculated independetly for each line !!!
	vec2 Font::calculate_text_size(const std::string &text, const vec2 &size) const {
		vec2 textSize = {0, m_FontHeight * size.y};
		f32 tempWidth = 0.0f;

		for(std::string::const_iterator it = text.begin(); it < text.end(); ++it) {
			const bool isLastChar = it == text.end() - 1;

			switch(*it) {
			case '\\':
				if(isLastChar)
					break;

				switch(*(++it)) {
				case 'n':
					textSize.y += m_FontHeight * size.y;
					tempWidth = 0.0f;
					break;
				}

				break;

			default:
				const Glyph &glyph = get_glyph(*it);
				tempWidth += glyph.advance.x * size.x;

				break;
			}

			if(tempWidth > textSize.x)
				textSize.x = tempWidth;
		}

		return textSize;
	}
}// namespace fif::gfx
