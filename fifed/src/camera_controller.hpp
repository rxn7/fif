#pragma once

namespace fifed {
	class CameraController {
	public:
		static void update();
		static void on_event(Event &event, bool viewportHovered);

		static f32 s_MaxZoom;
		static f32 s_MinZoom;
		static f32 s_ZoomLerpDuration;
	};
}// namespace fifed
