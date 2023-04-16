#include "fif/gfx/orthoCamera.h"

#include "fif/core/application.h"
#include "fif/core/profiler.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"
#include "GLFW/glfw3.h"
#include <iostream>

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {
	}

	void OrthoCamera::update() {
		FIF_PROFILE_FUNC();
		std::int32_t width, height;
		glfwGetWindowSize(core::Application::getInstance().getWindow().getGlfwWindow(), &width, &height);

		const float aspect = static_cast<float>(height) / static_cast<float>(width);
		const glm::vec2 size(m_Size, m_Size * aspect);

		m_CameraMatrix = glm::ortho(-size.x + m_Position.x, size.x + m_Position.x, -size.y + m_Position.y, size.y + m_Position.y);
	}
}
