#include "fif/gfx/orthoCamera.h"

#include "fif/core/application.h"
#include "fif/core/profiler.h"
#include "fif/core/opengl.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {
	}

	void OrthoCamera::update() {
		FIF_PROFILE_FUNC();

		const glm::i32vec2 windowSize = core::Application::getInstance().getWindow().getSize();

		const float aspect = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);
		const glm::vec2 size = glm::vec2(SIZE, SIZE * aspect) * m_Zoom;

		m_CameraMatrix = glm::ortho(-size.x + m_Position.x, size.x + m_Position.x, -size.y + m_Position.y, size.y + m_Position.y);
		m_CameraMatrixInverse = glm::inverse(m_CameraMatrix);
	}

	glm::vec2 OrthoCamera::screenToWorld(const glm::vec2 &position) const {
		FIF_PROFILE_FUNC();

		const glm::i16vec2 windowSize = core::Application::getInstance().getWindow().getSize();
		const glm::vec2 normalizedPosition((position.x * 2.0f) / windowSize.x - 1.0f, 1.0f - (2.0f * position.y) / windowSize.y);

		return m_CameraMatrixInverse * glm::vec4(normalizedPosition, 0.0f, 1.0f);
	}
}
