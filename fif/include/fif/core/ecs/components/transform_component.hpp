#pragma once

namespace fif::gfx {
	struct TransformComponent final {
		glm::vec2 position = {0.0f, 0.0f};
		glm::vec2 scale = {1.0f, 1.0f};
		f32 angleRadians = 0.0f;
	};
}// namespace fif::gfx
