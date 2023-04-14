#pragma once

#include <iostream>

#define FIF_ASSERT(cond, msg) \
	if(!(cond)) { \
		std::cerr << "Assertion failed: [" << #cond << "] in function: [" << __FUNCTION__ << "], line: " << __LINE__ << std::endl; \
		std::abort(); \
	}
