#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/texture.hpp"

namespace fif::gfx {
	struct SpriteComponent final {
		SpriteComponent() {}

		i8 zIndex = 0;

		Color tint = Colors::WHITE;
		vec2 size = {100.0f, 100.0f};

		std::shared_ptr<Texture> p_Texture = nullptr;
	};
}// namespace fif::gfx
