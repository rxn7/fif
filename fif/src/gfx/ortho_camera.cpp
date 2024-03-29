#include "fif/gfx/ortho_camera.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fif::gfx {
	OrthoCamera::~OrthoCamera() {}

	void OrthoCamera::update() {
		update_size();
		m_CameraMatrix = glm::ortho(-m_Size.x + m_Position.x, m_Size.x + m_Position.x, -m_Size.y + m_Position.y, m_Size.y + m_Position.y);
	}

	void OrthoCamera::update_size() {
		const vec2 viewportSize = GfxModule::get_instance().get_viewport_size();
		const f32 aspect = static_cast<f32>(viewportSize.x) / static_cast<f32>(viewportSize.y);
		m_Size = vec2(aspect, 1.0f) * m_Zoom * BASE_ZOOM;
	}

	vec2 OrthoCamera::screen_to_world(const vec2 &position) const {
		const vec2 viewportSize = GfxModule::get_instance().get_viewport_size();
		const vec2 relativePosition = position - GfxModule::get_instance().get_viewport_position();
		const vec2 normalizedPosition((relativePosition.x * 2.0f) / viewportSize.x - 1.0f, 1.0f - (2.0f * relativePosition.y) / viewportSize.y);

		return normalizedPosition * m_Size + m_Position;
	}
}// namespace fif::gfx
