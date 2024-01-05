#pragma once

#include "fif/gfx/color.hpp"
#include "fif/gfx/texture.hpp"

namespace fif::gfx {
	struct SpriteComponent final {
		SpriteComponent() {
		}

		SpriteComponent(const std::shared_ptr<Texture> &texture) {
			set_texture(texture);
		}

		void set_texture(const std::shared_ptr<Texture> &texture) {
			p_texture = texture;
			size = texture->get_size();
		}

		Color tint = Colors::WHITE;
		vec2 size = {100.0f, 100.0f};

		std::shared_ptr<Texture> p_texture = nullptr;
	};
}// namespace fif::gfx
