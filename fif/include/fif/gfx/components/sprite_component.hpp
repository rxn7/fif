#pragma once

#include "fif/gfx/texture.hpp"

namespace fif::gfx {
	struct SpriteComponent final {
		SpriteComponent() {}
		SpriteComponent(std::shared_ptr<Texture> texture) : p_texture(texture) {}

		Color tint = Colors::WHITE;
		glm::vec2 size = {100.0f, 100.0f};

		std::shared_ptr<Texture> p_texture = nullptr;
	};
}// namespace fif::gfx
