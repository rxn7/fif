#pragma once

#include "scopeTimer.h"
#include "functionName.h"

#include <vector>

namespace fif::core {
	namespace Profiler {
		void addResult(const fif::core::TimerResult &result);
		const std::vector<fif::core::TimerResult> &getResults();
		void clear();
	}
}

#define FIF_PROFILE_FUNC() fif::core::ScopeTimer(__FUNCTION_NAME__, [&](fif::core::TimerResult result) { fif::core::Profiler::addResult(result); })
