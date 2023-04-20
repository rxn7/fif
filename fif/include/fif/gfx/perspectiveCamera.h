#pragma once

#include "fif/gfx/camera.h"

namespace fif::gfx {
	class PerspectiveCamera : public Camera {
	  public:
		~PerspectiveCamera();

		void update() override;

	  public:
		glm::vec3 m_Position, m_Direction;
		float m_Fov = 45.0f;
		float m_ZNear = 0.1f;
		float m_ZFar = 100.0f;

	  protected:
		glm::mat4 m_ViewMatrix, m_ProjectionMatrix;
	};
} // namespace fif::gfx
