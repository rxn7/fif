#include "fif/core/util/timing.h"
#include "fif/core/opengl.h"
#include "fif/core/util/clock.h"
#include "fif/core/util/log.h"
#include <iomanip>

using namespace std::chrono;

namespace fif::core {
	static Clock::time_point m_LastFrameTime;
	float Time::m_Dt;

	void Time::update() {
		const auto now = Clock::now();
		m_Dt = duration_cast<duration<float>>(Clock::now() - m_LastFrameTime).count();
		m_LastFrameTime = now;
	}
} // namespace fif::core
