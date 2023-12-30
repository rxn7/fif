#include "fif/gfx/text/font.hpp"

namespace fif::gfx {
	Font::Font(const std::shared_ptr<Texture> &texture, const f32 size) : mp_Texture(texture), m_Size(size) {
	}

	Font::~Font() {
	}

	// TODO: Width should be calculated independetly for each line !!!
	vec2 Font::calculate_text_size(const std::string &text, const f32 fontSize, const f32 lineHeight, const f32 charSpacing) {
		vec2 size{0, lineHeight};
		f32 tempWidth = 0.0f;

		for(std::string::const_iterator it = text.begin(); it < text.end(); ++it) {
			const bool isLastChar = it == text.end() - 1;

			switch(*it) {
			case '\\':
				if(isLastChar)
					break;

				switch(*(++it)) {
				case 'n':
					size.y += lineHeight;
					tempWidth = 0.0f;
					break;
				}

				break;

			default:
				tempWidth += fontSize;

				if(!isLastChar)
					tempWidth += charSpacing;

				break;
			}

			if(tempWidth > size.x)
				size.x = tempWidth;
		}

		return size;
	}
}// namespace fif::gfx
