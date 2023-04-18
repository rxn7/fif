#include "fif/core/profiler.h"

static std::vector<fif::core::TimerResult> s_Results;
static std::vector<fif::core::TimerResult> s_TempResults;

void fif::core::Profiler::beginFrame() {
	s_Results = s_TempResults;
	s_TempResults.clear();
}

void fif::core::Profiler::addResult(const TimerResult &result) {
	s_TempResults.push_back(result);
}

const std::vector<fif::core::TimerResult> &fif::core::Profiler::getResults() {
	return s_Results;
}
