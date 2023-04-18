#include "fif/core/util/timing.h"
#include "fif/core/util/clock.h"

using namespace std::chrono;

namespace fif::core {
	static Clock::time_point m_LastFrameTime;
	float Time::m_Dt;

	void Time::update() {
		const Clock::time_point now = Clock::now();
		m_Dt = duration_cast<nanoseconds>(now - m_LastFrameTime).count() * 0.000000001f;
		m_LastFrameTime = now;
	}
} // namespace fif::core
