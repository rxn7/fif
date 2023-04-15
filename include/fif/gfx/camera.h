#pragma once

#include "glm/mat4x4.hpp"

namespace fif::gfx {
	class Camera {
	public:
		Camera(bool ortho = true);
		void update();

		inline const glm::mat4 &getViewMatrix() const {
			return m_ViewMatrix;
		}

		inline const glm::mat4 &getProjectionMatrix() const {
			return m_ProjectionMatrix;
		}

		inline const glm::mat4 &getViewProjectionMatrix() const {
			return m_ViewProjectionMatirx;
		}
	
	public:
		glm::vec3 m_Position, m_Direction;
		bool m_Ortho;
		float m_Fov = 45.0f;
		float m_ZNear = 0.1f;
		float m_ZFar = 100.0f;

	private:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix, m_ViewProjectionMatirx;
	};
}
