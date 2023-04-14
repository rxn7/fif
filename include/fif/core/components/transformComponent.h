#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

namespace fif::core {
	struct TransformComponent {
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	};
}
