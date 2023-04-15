#pragma once 

#include "fif/gfx/camera.h"

namespace fif::gfx {
	class OrthoCamera : public Camera {
	public:
		~OrthoCamera();

		void update() override;

	public:
		float m_Size = 100.0f;
		glm::vec2 m_Position;
	};
}
