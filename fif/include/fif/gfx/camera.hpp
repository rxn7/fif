#pragma once

#include "fif/core/util/log.hpp"

#include "glm/mat4x4.hpp"

namespace fif::gfx {
	class Camera {
	public:
		virtual ~Camera() {}

		virtual void update() {}

		inline const glm::mat4 &get_matrix() const { return m_CameraMatrix; }

		inline void set_viewport(const glm::vec2 &size, const glm::vec2 &pos = {0, 0}) {
			m_ViewportSize = size;
			m_ViewportPosition = pos;
		}

	protected:
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportPosition;
		glm::mat4 m_CameraMatrix;
	};
}// namespace fif::gfx
