#pragma once

#include "event/event.hpp"
#include "fif/gfx/color.hpp"
#include "glm/vec2.hpp"

namespace CameraController {
	void update();
	void on_event(fif::core::Event &event);

	inline float maxZoom = 100.0f;
	inline float minZoom = 0.005f;
	inline float zoomLerpSpeed = 20.0f;

	inline glm::vec2 viewportPosition;

	inline glm::vec2 viewportSize;

	inline glm::vec2 targetPosition = {0, 0};
	inline float targetZoom = 1.0f;

	inline bool isZooming = false;
};// namespace CameraController
