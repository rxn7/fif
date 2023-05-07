#pragma once

namespace fif::gfx {
	struct QuadComponent : core::Component {
		Color tint = Colors::WHITE;
		glm::vec2 size = {50.0f, 50.0f};
	};
}// namespace fif::gfx
