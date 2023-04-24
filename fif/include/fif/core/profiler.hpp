#pragma once

#include "fif/core/util/function_name.hpp"
#include "fif/core/util/scope_timer.hpp"

#include <vector>

namespace fif::core {
	namespace Profiler {
		void begin_frame();
		void add_result(const fif::core::TimerResult &result);
		const std::vector<fif::core::TimerResult> &get_results();
	}// namespace Profiler
}// namespace fif::core

#ifdef FIF_PROFILING_ENABLED
#define FIF_PROFILE_FUNC()                                                                                                                           \
	fif::core::ScopeTimer __timer##__LINE__ =                                                                                                        \
		fif::core::ScopeTimer(FIF_FUNCTION_NAME, [&](fif::core::TimerResult result) { fif::core::Profiler::add_result(result); })
#else
#define FIF_PROFILE_FUNC()
#endif
