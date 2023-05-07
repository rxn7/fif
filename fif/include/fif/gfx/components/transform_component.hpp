#pragma once

namespace fif::gfx {
	struct TransformComponent : core::Component {
		glm::vec2 position;
		f32 angle;
	};
}// namespace fif::gfx
