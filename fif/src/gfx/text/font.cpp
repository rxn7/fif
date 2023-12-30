#include "fif/gfx/text/font.hpp"

namespace fif::gfx {
	Font::Font(const std::shared_ptr<Texture> &texture, i32 filter, f32 size) : m_Size(size), m_Texture(texture) {
	}
}// namespace fif::gfx
