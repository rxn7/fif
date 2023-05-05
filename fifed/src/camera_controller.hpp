#pragma once

#include "common.hpp"

namespace fifed::CameraController {
	void update();
	void on_event(Event &event, bool viewportHovered);

	inline float maxZoom = 100.0f;
	inline float minZoom = 0.005f;
	inline float zoomLerpSpeed = 20.0f;

	inline glm::vec2 viewportPosition;

	inline glm::vec2 viewportSize;

	inline glm::vec2 targetPosition = {0, 0};
	inline float targetZoom = 1.0f;

	inline bool isZooming = false;
};// namespace fifed::CameraController
