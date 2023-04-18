#include "fif/gfx/orthoCamera.h"

#include "fif/core/application.h"
#include "fif/core/opengl.h"
#include "fif/core/profiler.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {}

	void OrthoCamera::update() {
		FIF_PROFILE_FUNC();

		const glm::i32vec2 windowSize = core::Application::getInstance().getWindow().getSize();
		const float aspect = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);

		m_Size = glm::vec2(SIZE, SIZE * aspect) * m_Zoom;

		m_CameraMatrix = glm::ortho(-m_Size.x + m_Position.x, m_Size.x + m_Position.x, -m_Size.y + m_Position.y, m_Size.y + m_Position.y);
	}

	glm::vec2 OrthoCamera::screenToWorld(const glm::vec2 &position) const {
		FIF_PROFILE_FUNC();

		const glm::i16vec2 windowSize = core::Application::getInstance().getWindow().getSize();
		const glm::vec2 normalizedPosition((position.x * 2.0f) / windowSize.x - 1.0f, 1.0f - (2.0f * position.y) / windowSize.y);

		return normalizedPosition * m_Size + m_Position;
	}

	bool OrthoCamera::containsCircle(const glm::vec2 &position, float radius) {
		// clang-format off
		return position.x + radius > m_Position.x - m_Size.x && 
			   position.x - radius < m_Position.x + m_Size.x && 
			   position.y + radius > m_Position.y - m_Size.y &&
			   position.y - radius < m_Position.y + m_Size.y;
		// clang-format on
	}

	bool OrthoCamera::containsQuad(const glm::vec2 &position, const glm::vec2 &size) {
		const glm::vec2 halfSize = size * 0.5f;

		// clang-format off
		return position.x + halfSize.x > m_Position.x - m_Size.x && 
			   position.x - halfSize.x < m_Position.x + m_Size.x && 
			   position.y + halfSize.y > m_Position.y - m_Size.y &&
			   position.y - halfSize.y < m_Position.y + m_Size.y;
		// clang-format on
	}
} // namespace fif::gfx
