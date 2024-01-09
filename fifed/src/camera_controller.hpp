#pragma once

namespace fifed {
	class CameraController final {
	public:
		void update();
		void on_event(Event &event);

	private:
		vec2 m_StartMousePositionLocal;
		f32 m_MaxZoom = 10000.0f;
		f32 m_MinZoom = 0.001f;
		f32 m_ZoomLerpDuration = 0.05f;
		bool m_IsZooming = false;
		f32 m_ZoomTimer = 0.0f;
		f32 m_StartZoom = 1.0f;
		f32 m_TargetZoom = 1.0f;

		friend class SettingsPanel;
	};
}// namespace fifed
