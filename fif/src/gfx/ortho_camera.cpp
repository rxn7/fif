#include "fif/gfx/ortho_camera.hpp"
#include "fif/core/application.hpp"
#include "fif/core/opengl.hpp"

#include "gfx_module.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/vec2.hpp"

#include <algorithm>

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {}

	void OrthoCamera::update() {
		update_size();
		m_CameraMatrix = glm::ortho(-m_Size.x + m_Position.x, m_Size.x + m_Position.x, -m_Size.y + m_Position.y, m_Size.y + m_Position.y);
	}

	void OrthoCamera::update_size() {
		const f32 aspect = static_cast<f32>(GfxModule::get_viewport_size().y) / static_cast<f32>(GfxModule::get_viewport_size().x);
		m_Size = glm::vec2(BASE_SIZE, BASE_SIZE * aspect) * m_Zoom;
	}

	glm::vec2 OrthoCamera::screen_to_world(const glm::vec2 &position) const {
		const glm::vec2 relativePosition = position - GfxModule::get_viewport_position();
		const glm::vec2 normalizedPosition(
			(relativePosition.x * 2.0f) / GfxModule::get_viewport_size().x - 1.0f, 1.0f - (2.0f * relativePosition.y) / GfxModule::get_viewport_size().y);

		return normalizedPosition * m_Size + m_Position;
	}

	bool OrthoCamera::contains_circle(const glm::vec2 &position, f32 radius) const {
		const glm::vec2 delta = glm::abs(m_Position - position);
		return delta.x <= m_Size.x + radius && delta.y <= m_Size.y + radius;
	}

	bool OrthoCamera::contains_quad(const glm::vec2 &position, const glm::vec2 &size) const {
		const glm::vec2 halfSize = size * 0.5f;

		// TODO: What about rotation?

		// clang-format off
		return position.x + halfSize.x > m_Position.x - m_Size.x && 
			   position.x - halfSize.x < m_Position.x + m_Size.x && 
			   position.y + halfSize.y > m_Position.y - m_Size.y &&
			   position.y - halfSize.y < m_Position.y + m_Size.y;
		// clang-format on
	}
}// namespace fif::gfx
