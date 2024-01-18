#pragma once

struct PerformanceStats final {
	f32 frameTimeMs;
	f32 cpuTimeMs;
	f32 gpuTimeMs;
	u32 fps;
	u32 frameCount;
};
