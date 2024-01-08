#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/texture.hpp"

namespace fif::gfx {
	struct SpriteComponent final {
		SpriteComponent() {}

		Color tint = Colors::WHITE;
		vec2 size = {100.0f, 100.0f};

		std::shared_ptr<Texture> p_texture = nullptr;
	};
}// namespace fif::gfx
