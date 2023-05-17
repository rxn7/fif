#pragma once

#define FIF_ASSERT(cond, msg, ...) fif::core::assert_impl(cond, #cond, __FILE_NAME__, FIF_FUNCTION_NAME, __LINE__, msg, ##__VA_ARGS__);

namespace fif::core {
	template<typename... Args>
	void assert_impl(bool cond, const char *condStr, const char *file, const char *function, int line, const char *msg, Args... args) {
		if(cond)
			return;

		fif::core::Logger::error("Assertion failed: [%s] in file: %s, function: [%s], line: %i", condStr, file, function, line);
		fif::core::Logger::error(msg, std::forward<Args>(args)...);
		std::abort();
	}
}// namespace fif::core