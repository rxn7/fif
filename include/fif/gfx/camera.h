#pragma once

#include "glm/ext/matrix_float4x4.hpp"
#include "glm/mat4x4.hpp"

namespace fif::gfx {
	class Camera {
	public:
		virtual ~Camera() {}

		virtual void update() {}

		inline const glm::mat4 &getMatrix() const {
			return m_CameraMatrix;
		}

	protected:
		glm::mat4 m_CameraMatrix;
	};
}
