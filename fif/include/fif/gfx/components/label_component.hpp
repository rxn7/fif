#pragma once

namespace fif::gfx {
	struct LabelComponent final {
		f32 size = 12.0f;
		Color color = Colors::BLACK;
		std::string text = "";
		// TODO: Font resource.
	};
}// namespace fif::gfx
