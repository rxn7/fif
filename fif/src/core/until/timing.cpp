#include <iomanip>

using namespace std::chrono;

namespace fif::core {
	static Clock::time_point m_LastFrameTime;
	float Timing::m_Dt;

	void Timing::init() {
		m_LastFrameTime = Clock::now();
	}

	void Timing::update() {
		const auto now = Clock::now();
		m_Dt = duration_cast<duration<float>>(Clock::now() - m_LastFrameTime).count();
		m_LastFrameTime = now;
	}
}// namespace fif::core
