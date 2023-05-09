#pragma once

#define FIF_ASSERT(cond, msg, ...)                                                                                                                   \
	if(!(cond)) {                                                                                                                                    \
		fif::core::Logger::error("Assertion failed: [%s] in file: %s, function: [%s], line: \n%s", #cond, __FILE__, FIF_FUNCTION_NAME, __LINE__);    \
		fif::core::Logger::error(msg, ##__VA_ARGS__);                                                                                                \
		std::abort();                                                                                                                                \
	}
