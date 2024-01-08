#pragma once

#include "fif/core/invokable.hpp"

namespace fif::core {
	class Logger final {
	public:
		enum class LogType : u8 { Info, Warn, Error, Debug };

		typedef void (*LoggerCallback)(LogType, const char *msg);

		template<LogType> static void print_prefix();

		template<LogType T, typename... Args> static void print(std::string_view msg, Args... args) {
			print_prefix<T>();

			const size_t size = std::snprintf(NULL, 0, msg.data(), std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			std::snprintf(buf, size, msg.data(), std::forward<Args>(args)...);

			std::fputs(buf, stdout);
			std::puts("\033[0m");

			s_LoggerHook.invoke(T, buf);

			std::free(buf);
		}

#define FIF_PRINT_FUNC(name, logType)                                                                                                                                                                  \
	template<typename... Args> static void name(std::string_view msg, Args... args) { print<logType>(msg, std::forward<Args>(args)...); }

		FIF_PRINT_FUNC(info, LogType::Info)
		FIF_PRINT_FUNC(warn, LogType::Warn)
		FIF_PRINT_FUNC(error, LogType::Error)
		FIF_PRINT_FUNC(debug, LogType::Debug)

	public:
		inline static Invokable<LogType, const char *> s_LoggerHook;
	};
};// namespace fif::core
