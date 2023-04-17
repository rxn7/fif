#pragma once

#include <string>
#include <chrono>

#include "fif/core/log.h"
#include "fif/core/clock.h"

namespace fif::core {
	struct TimerResult {
		std::string name;
		float durationMs;
	};

	template<typename Func>
	class ScopeTimer {
	public:
		ScopeTimer(const std::string &name, Func &&func) : m_Name(name), m_BeginTime(Clock::now()), m_Func(func) {
		}

		~ScopeTimer() {
			using namespace std::chrono;
			const float durationMs = duration_cast<nanoseconds>((Clock::now() - m_BeginTime)).count() * 0.0000001f;
			m_Func(TimerResult{m_Name, durationMs});
		}
	
	private:
		std::string m_Name;
		Clock::time_point m_BeginTime;
		Func m_Func;
	};
}
