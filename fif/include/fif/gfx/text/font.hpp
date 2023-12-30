#pragma once

#include "fif/gfx/texture.hpp"

namespace fif::gfx {
	class Font {// TODO: Extend resource ?
	public:
		Font(const std::shared_ptr<Texture> &texture, const f32 size = 2.0f);

		virtual ~Font();
		void get_char_uv(char c, vec2 &start, vec2 &end) const;
		static vec2 calculate_text_size(const std::string &text, const f32 size, const f32 lineHeight, const f32 charSpacing);

		inline const Texture &get_texture() const {
			return *mp_Texture;
		}

		inline f32 get_char_width() const {
			return m_CharSize.x;
		}

		inline f32 get_char_height() const {
			return m_CharSize.y;
		}

	private:
		std::shared_ptr<Texture> mp_Texture;
		f32 m_Size;
		vec2 m_CharSize;
		vec2 m_CharSizeNormalized;
	};
}// namespace fif::gfx
