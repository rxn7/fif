#pragma once

#include "fif/core/util/log.hpp"

#include "glm/mat4x4.hpp"

namespace fif::gfx {
	class Camera {
	public:
		virtual ~Camera() {}

		virtual void update() {}

		inline const glm::mat4 &get_matrix() const { return m_CameraMatrix; }

	protected:
		glm::mat4 m_CameraMatrix;
	};
}// namespace fif::gfx
