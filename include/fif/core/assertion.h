#pragma once

#include <iostream>
#include "fif/core/log.h"
#include "fif/core/functionName.h"

#define FIF_ASSERT(cond, msg) \
	if(!(cond)) { \
		FIF_LOG_ERROR("Assertion failed: [" << #cond << "] in file: " << __FILE__ << ", function: [" << __FUNCTION_NAME__ << "], line: " << __LINE__ << "\n" << msg << std::endl); \
		std::abort(); \
	}
