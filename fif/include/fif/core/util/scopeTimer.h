#pragma once

#include <chrono>
#include <string>

#include "fif/core/util/clock.h"
#include "fif/core/util/log.h"

namespace fif::core {
	struct TimerResult {
		std::string name;
		float durationMs;
	};

	template <typename Func> class ScopeTimer {
	  public:
		ScopeTimer(const std::string &name, Func &&func)
			: m_Name(name), m_BeginTime(Clock::now()), m_Func(func) {}

		~ScopeTimer() {
			using namespace std::chrono;
			const float durationMs =
				duration_cast<duration<float, std::milli>>((Clock::now() - m_BeginTime)).count();
			m_Func(TimerResult{m_Name, durationMs});
		}

	  private:
		std::string m_Name;
		Clock::time_point m_BeginTime;
		Func m_Func;
	};
} // namespace fif::core
