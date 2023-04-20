#include "fif/core/util/timing.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/util/log.hpp"
#include <iomanip>

using namespace std::chrono;

namespace fif::core {
	static Clock::time_point m_LastFrameTime;
	float Timing::m_Dt;

	void Timing::update() {
		const auto now = Clock::now();
		m_Dt = duration_cast<duration<float>>(Clock::now() - m_LastFrameTime).count();
		m_LastFrameTime = now;
	}
} // namespace fif::core
