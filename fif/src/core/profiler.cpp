#include "fif/core/profiler.hpp"

static std::vector<fif::core::TimerResult> s_Results;
static std::vector<fif::core::TimerResult> s_TempResults;

void fif::core::Profiler::begin_frame() {
	s_Results = s_TempResults;
	s_TempResults.clear();
}

void fif::core::Profiler::add_result(const TimerResult &result) {
	s_TempResults.push_back(result);
}

const std::vector<fif::core::TimerResult> &fif::core::Profiler::get_results() {
	return s_Results;
}
