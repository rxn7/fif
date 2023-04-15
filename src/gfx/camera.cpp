#include "fif/gfx/camera.h"

#include "fif/core/application.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "GLFW/glfw3.h"

namespace fif::gfx {
	Camera::Camera(bool ortho) : m_Direction(0.0f, 0.0f, 1.0f), m_Ortho(ortho) {
	}

	void Camera::update() {
		std::int32_t width, height;
		glfwGetWindowSize(core::Application::getInstance().getWindow().getGlfwWindow(), &width, &height);

		if(m_Ortho) {
			const float hw = width * 0.5f;
			const float hh = height * 0.5f;
			m_ViewMatrix = glm::ortho(-hw + m_Position.x, hw + m_Position.x, -hh + m_Position.y, hh + m_Position.y);

			m_ViewProjectionMatirx = m_ViewMatrix;
		} else {
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), static_cast<float>(width) / static_cast<float>(height), m_ZNear, m_ZFar);
			m_Direction = glm::normalize(m_Direction);

			const glm::vec3 up(0.0f, 0.0f, 1.0f);
			m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, up);

			m_ViewProjectionMatirx = m_ProjectionMatrix * m_ViewMatrix;
		}
	}
}
