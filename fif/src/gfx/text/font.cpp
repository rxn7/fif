#include "fif/gfx/text/font.hpp"
#include "fif/core/util/logger.hpp"
#include "fif/gfx/gfx_module.hpp"

#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fif::gfx {
	Font::Font(const std::string &path, const u32 size, const GLint filter) : m_Size(size) {
		if(FT_Error error = FT_New_Face(GfxModule::get_instance()->m_FreeType, path.c_str(), 0, &m_Face)) {
			core::Logger::error("Failed to load freetype font: %s", FT_Error_String(error));
			return;
		}

		FT_Set_Pixel_Sizes(m_Face, 0, size);
		const u32 glyphPadding = 2u;
		u32 row = 0;
		u32 col = glyphPadding;
		u32 textureSize = 512;
		u8 *textureBuffer = new u8[textureSize * textureSize];

		for(char c = 32; c < 127; ++c) {
			const u32 glyphIdx = FT_Get_Char_Index(m_Face, c);

			FT_Load_Glyph(m_Face, glyphIdx, FT_LOAD_DEFAULT);
			FT_Error error = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
			FIF_ASSERT(error == 0, "Failed to render glyph");

			// Move the glyph to the next row if it won't fix on this row.
			if(col + m_Face->glyph->bitmap.width + glyphPadding >= textureSize) {
				col = glyphPadding;
				row += size;
			}

			m_FontHeight = std::max(m_FontHeight, static_cast<f32>((m_Face->size->metrics.ascender - m_Face->size->metrics.descender) >> 6));

			for(u32 y = 0; y < m_Face->glyph->bitmap.rows; ++y) {
				for(u32 x = 0; x < m_Face->glyph->bitmap.width; ++x) {
					textureBuffer[(row + y) * textureSize + col + x] = m_Face->glyph->bitmap.buffer[y * m_Face->glyph->bitmap.width + x];
				}
			}

			Glyph glyph;
			glyph.startUv = {static_cast<f32>(col) / textureSize, static_cast<f32>(row) / textureSize};
			glyph.endUv = {static_cast<f32>(col + m_Face->glyph->bitmap.width) / textureSize, static_cast<f32>(row + m_Face->glyph->bitmap.rows) / textureSize};
			glyph.size = {m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows};
			glyph.advance = {m_Face->glyph->advance.x >> 6, m_Face->glyph->advance.y >> 6};
			glyph.offset = {m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top};
			m_Glyphs.insert({c, glyph});

			col += m_Face->glyph->bitmap.width + glyphPadding;
		}

		FT_Done_Face(m_Face);

		mp_Texture = std::make_shared<Texture>(textureSize, textureSize, GL_R8, GL_RED, filter, GL_REPEAT, textureBuffer);
		delete[] textureBuffer;

		core::Logger::info("Font %s has been loaded", path.c_str());
	}

	Font::~Font() {
	}

	// TODO: Width should be calculated independetly for each line !!!
	vec2 Font::calculate_text_size(const std::string &text, const f32 fontSize) const {
		vec2 size = {0, m_FontHeight * fontSize};
		f32 tempWidth = 0.0f;

		for(std::string::const_iterator it = text.begin(); it < text.end(); ++it) {
			const bool isLastChar = it == text.end() - 1;

			switch(*it) {
			case '\\':
				if(isLastChar)
					break;

				switch(*(++it)) {
				case 'n':
					size.y += m_FontHeight * fontSize;
					tempWidth = 0.0f;
					break;
				}

				break;

			default:
				const Glyph &glyph = get_glyph(*it);
				tempWidth += glyph.advance.x * fontSize;

				break;
			}

			if(tempWidth > size.x)
				size.x = tempWidth;
		}

		return size;
	}
}// namespace fif::gfx
