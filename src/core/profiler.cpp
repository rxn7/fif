#include "fif/core/profiler.h"

static std::vector<fif::core::TimerResult> s_Results;

void fif::core::Profiler::clear() {
	s_Results.clear();
}

void fif::core::Profiler::addResult(const TimerResult &result) {
	s_Results.push_back(result);
}

const std::vector<fif::core::TimerResult> &fif::core::Profiler::getResults() {
	return s_Results;
}
