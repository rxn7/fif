#pragma once

#include "glm/mat4x4.hpp"

namespace fif::core {
	namespace Transform {
		glm::mat4 getModelMatrix(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation);
	}
}
