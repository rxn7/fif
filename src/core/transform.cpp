#include "fif/core/transform.h"
#include "glm/ext/matrix_transform.hpp"

namespace fif::core {
	glm::mat4 Transform::getModelMatrix(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation) {
		glm::mat4 matrix(1.0f);

		matrix = glm::translate(matrix, position);
		matrix = glm::rotate(matrix, rotation[0], {1,0,0});
		matrix = glm::rotate(matrix, rotation[1], {0,1,0});
		matrix = glm::rotate(matrix, rotation[2], {0,0,1});
		matrix = glm::scale(matrix, scale);

		return matrix;
	}
}
