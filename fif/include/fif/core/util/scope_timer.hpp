#pragma once

namespace fif::core {
	struct TimerResult {
		std::string name;
		float durationMs;
	};

	class ScopeTimer {
	public:
		typedef std::function<void(const TimerResult &result)> CallbackFunc;

		ScopeTimer(const std::string &name, CallbackFunc callback) : m_Name(name), m_BeginTime(Clock::now()), m_Callback(callback) {}

		~ScopeTimer() {
			const float durationMs = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>((Clock::now() - m_BeginTime)).count();
			m_Callback(TimerResult{m_Name, durationMs});
		}

	private:
		std::string m_Name;
		Clock::time_point m_BeginTime;
		CallbackFunc m_Callback;
	};
}// namespace fif::core
