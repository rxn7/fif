#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/camera.hpp"

namespace fif::gfx {
	class OrthoCamera : public Camera {
	public:
		~OrthoCamera();

		void update() override;
		glm::vec2 screen_to_world(const glm::vec2 &position) const;
		void update_size();

		inline glm::vec2 get_size() const { return m_Size; }

	public:
		static constexpr f32 BASE_ZOOM = 500.0f;
		f32 m_Zoom = 1.0f;
		glm::vec2 m_Position;

	private:
		glm::vec2 m_Size;
		f32 m_Aspect;
	};
}// namespace fif::gfx
