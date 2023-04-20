#include "fif/gfx/perspectiveCamera.h"

#include "fif/core/application.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"

#include "GLFW/glfw3.h"

namespace fif::gfx {
	PerspectiveCamera::~PerspectiveCamera() {}

	void PerspectiveCamera::update() {
		std::int32_t width;
		std::int32_t height;
		glfwGetWindowSize(core::Application::getInstance().getWindow().getGlfwWindow(), &width,
						  &height);

		m_ProjectionMatrix = glm::perspective(
			glm::radians(m_Fov), static_cast<float>(width) / static_cast<float>(height), m_ZNear,
			m_ZFar);
		m_Direction = glm::normalize(m_Direction);

		const glm::vec3 up(0.0F, 0.0F, 1.0F);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, up);

		m_CameraMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
} // namespace fif::gfx
