#pragma once

#include "fif/core/util/functionName.h"
#include "fif/core/util/scopeTimer.h"

#include <vector>

namespace fif::core {
	namespace Profiler {
		void beginFrame();
		void addResult(const fif::core::TimerResult &result);
		const std::vector<fif::core::TimerResult> &getResults();
	} // namespace Profiler
} // namespace fif::core

#ifdef FIF_PROFILING_ENABLED
#define FIF_PROFILE_FUNC() fif::core::ScopeTimer __timer##__LINE__ = fif::core::ScopeTimer(FIF_FUNCTION_NAME, [&](fif::core::TimerResult result) { fif::core::Profiler::addResult(result); })
#else
#define FIF_PROFILE_FUNC()
#endif