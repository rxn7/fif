#include "fif/core/transform.h"
#include "glm/ext/matrix_transform.hpp"

namespace fif::core {
	glm::mat4 Transform::getModelMatrix(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &rotation) {
		glm::mat4 matrix(1.0f);

		/*
		glm::mat4 rotationMatrix(1.0f);
		glm::rotate(rotationMatrix, rotation[0], {1, 0, 0});
		glm::rotate(rotationMatrix, rotation[1], {0, 1, 0});
		glm::rotate(rotationMatrix, rotation[2], {0, 0, 1});
		*/

		matrix = glm::scale(matrix, scale);
		matrix = glm::translate(matrix, position);

		return matrix;
	}
}
