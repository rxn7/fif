#pragma once

#include <cinttypes>

struct RendererStats {
	std::uint32_t primitivesRendered;
	float frameTimeMs;
};
