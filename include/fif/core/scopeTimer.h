#pragma once

#include <string>
#include <chrono>

namespace fif::core {
	struct TimerResult {
		std::string name;
		float durationMs;
	};

	template<typename Func>
	class ScopeTimer {
	public:
		ScopeTimer(const std::string &name, Func &&func) : m_Name(name), m_Func(func) {
			m_BeginTime = std::chrono::high_resolution_clock::now();
		}

		~ScopeTimer() {
			float durationMs = std::chrono::duration<float, std::milli>((std::chrono::high_resolution_clock::now() - m_BeginTime)).count();
			m_Func(TimerResult{m_Name, durationMs});
		}
	
	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_BeginTime;
		Func m_Func;
	};
}
