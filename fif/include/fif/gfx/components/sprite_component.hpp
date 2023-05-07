#pragma once

#include "fif/gfx/texture.hpp"

namespace fif::gfx {
	struct SpriteComponent final {
		SpriteComponent() {}
		SpriteComponent(const std::string &path) : p_texture(std::make_shared<Texture>(path, GL_NEAREST)) {}
		SpriteComponent(const std::shared_ptr<Texture> &texture) : p_texture(texture) {}

		Color tint = Colors::WHITE;
		glm::vec2 size = {50, 50};

		// TODO: TextureAsset
		char path[1024];
		std::shared_ptr<Texture> p_texture = nullptr;
	};
}// namespace fif::gfx
