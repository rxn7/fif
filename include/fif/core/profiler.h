#pragma once

#include "scopeTimer.h"
#include "functionName.h"

#include <vector>
#include <source_location>

namespace fif::core {
	namespace Profiler {
		void addResult(const fif::core::TimerResult &result);
		const std::vector<fif::core::TimerResult> &getResults();
		void clear();
	}
}

#define FIF_PROFILE_FUNC() fif::core::ScopeTimer(std::source_location::current().function_name(), [&](fif::core::TimerResult result) { fif::core::Profiler::addResult(result); })
