#pragma once

#include "common.hpp"
#include "fif/gfx/color.hpp"

namespace Grid {
	void init();
	void render();

	inline bool enabled = true;
	inline Color lineColor = Color(20, 20, 20, 255);
	inline f32 lineThickness = 0.7f;
	inline f32 cellSize = 10.0f;
	inline f32 wrapValue = 1000.0f;
};// namespace Grid
