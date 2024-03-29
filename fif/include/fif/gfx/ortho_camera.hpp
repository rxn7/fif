#pragma once

#include "fif/gfx/camera.hpp"

namespace fif::gfx {
	class OrthoCamera final : public Camera {
	public:
		~OrthoCamera();

		void update() override;
		vec2 screen_to_world(const vec2 &position) const;
		void update_size();

		inline const vec2 &get_size() const { return m_Size; }

	public:
		static constexpr f32 BASE_ZOOM = 1000.0f;
		f32 m_Zoom = 1.0f;
		vec2 m_Position{0.0f, 0.0f};

	private:
		vec2 m_Size;
	};
}// namespace fif::gfx
