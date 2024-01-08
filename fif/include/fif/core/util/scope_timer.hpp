#pragma once

namespace fif::core {
	struct TimerResult final {
		std::string name;
		f32 durationMs;
	};

	class ScopeTimer final {
	public:
		typedef std::function<void(const TimerResult &result)> CallbackFunc;

		ScopeTimer(const std::string &name, CallbackFunc callback) : m_Name(name), m_BeginTime(Clock::now()), m_Callback(callback) {}

		~ScopeTimer() {
			const f32 durationMs = std::chrono::duration_cast<std::chrono::duration<f32, std::milli>>((Clock::now() - m_BeginTime)).count();
			m_Callback(TimerResult{m_Name, durationMs});
		}

	private:
		std::string m_Name;
		Clock::time_point m_BeginTime;
		CallbackFunc m_Callback;
	};
}// namespace fif::core
