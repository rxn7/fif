#pragma once

namespace fif::core {
	class Timer final {
	public:
		Timer() : m_StartTime(Clock::now()) {}

		inline f32 get_duration_ms() const { return std::chrono::duration_cast<std::chrono::duration<f32, std::milli>>((Clock::now() - m_StartTime)).count(); }
		inline void reset() { m_StartTime = Clock::now(); }

	private:
		Clock::time_point m_StartTime;
	};
}// namespace fif::core
