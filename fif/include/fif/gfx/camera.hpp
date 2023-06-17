#pragma once

namespace fif::gfx {
	class Camera {
	public:
		virtual ~Camera() {}

		virtual void update() {}

		inline const mat4 &get_matrix() const { return m_CameraMatrix; }

	protected:
		mat4 m_CameraMatrix;
	};
}// namespace fif::gfx
