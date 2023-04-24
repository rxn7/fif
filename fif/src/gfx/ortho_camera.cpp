#include "fif/gfx/ortho_camera.hpp"
#include "fif/core/application.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/profiler.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"

#include <algorithm>

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {}

	void OrthoCamera::update() {
		FIF_PROFILE_FUNC();

		update_size();

		m_CameraMatrix = glm::ortho(-m_Size.x + m_Position.x, m_Size.x + m_Position.x, -m_Size.y + m_Position.y, m_Size.y + m_Position.y);
	}

	void OrthoCamera::update_size() {
		const glm::i32vec2 windowSize = core::Application::get_instance().get_window().get_size();
		const float aspect = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);

		m_Size = glm::vec2(SIZE, SIZE * aspect) * m_Zoom;
	}

	glm::vec2 OrthoCamera::screen_to_world(const glm::vec2 &position) const {
		FIF_PROFILE_FUNC();

		const glm::i16vec2 windowSize = core::Application::get_instance().get_window().get_size();
		const glm::vec2 normalizedPosition((position.x * 2.0F) / windowSize.x - 1.0F, 1.0F - (2.0F * position.y) / windowSize.y);

		return normalizedPosition * m_Size + m_Position;
	}

	bool OrthoCamera::contains_circle(const glm::vec2 &position, float radius) const {
		const glm::vec2 delta = glm::abs(m_Position - position);
		return delta.x <= m_Size.x + radius && delta.y <= m_Size.y + radius;
	}

	bool OrthoCamera::contains_quad(const glm::vec2 &position, const glm::vec2 &size) const {
		const glm::vec2 halfSize = size * 0.5F;

		// TODO: What about rotation?

		// clang-format off
		return position.x + halfSize.x > m_Position.x - m_Size.x && 
			   position.x - halfSize.x < m_Position.x + m_Size.x && 
			   position.y + halfSize.y > m_Position.y - m_Size.y &&
			   position.y - halfSize.y < m_Position.y + m_Size.y;
		// clang-format on
	}
}// namespace fif::gfx
