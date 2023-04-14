#pragma once

#include <iostream>
#include "fif/core/log.h"

#define FIF_ASSERT(cond, msg) \
	if(!(cond)) { \
		FIF_LOG_ERROR("Assertion failed: [" << #cond << "] in function: [" << __FUNCTION__ << "], line: " << __LINE__); \
		std::abort(); \
	}
