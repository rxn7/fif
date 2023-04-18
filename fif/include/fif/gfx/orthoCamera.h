#pragma once

#include "fif/gfx/camera.h"
#include "glm/vec2.hpp"

namespace fif::gfx {
	class OrthoCamera : public Camera {
	  public:
		~OrthoCamera();

		void update() override;
		bool containsCircle(const glm::vec2 &position, float radius);
		bool containsQuad(const glm::vec2 &position, const glm::vec2 &size);
		glm::vec2 screenToWorld(const glm::vec2 &position) const;

	  public:
		static constexpr float SIZE = 500.0f;
		float m_Zoom = 1.0f;
		glm::vec2 m_Position;

	  private:
		glm::vec2 m_Size;
		float m_Aspect;
	};
} // namespace fif::gfx
