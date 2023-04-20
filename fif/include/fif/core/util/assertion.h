#pragma once

#include "fif/core/util/functionName.h"
#include "fif/core/util/log.h"
#include <iostream>

#define FIF_ASSERT(cond, msg)                                                                                                                                                                          \
	if (!(cond)) {                                                                                                                                                                                     \
		FIF_LOG_ERROR("Assertion failed: [" << #cond << "] in file: " << __FILE__ << ", function: [" << FIF_FUNCTION_NAME << "], line: " << __LINE__ << "\n" << msg << std::endl);                     \
		std::abort();                                                                                                                                                                                  \
	}