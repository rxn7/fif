#pragma once

namespace fif::core {
	class Logger {
	public:
		enum class LogType : u8 { INFO, WARN, ERROR, DEBUG };

		typedef void (*LoggerCallback)(LogType, const char *msg);

		static void add_callback(LoggerCallback callback);
		static void delete_callback(LoggerCallback callback);

		template<LogType> static void print_prefix();

		template<LogType T, typename... Args> static void print(std::string_view msg, Args... args) {
			print_prefix<T>();

			const size_t size = std::snprintf(NULL, 0, msg.data(), std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			std::snprintf(buf, size, msg.data(), std::forward<Args>(args)...);

			std::fputs(buf, stdout);
			std::puts("\033[0m");

			for(LoggerCallback &callback : s_Callbacks)
				callback(T, buf);

			std::free(buf);
		}

#define FIF_PRINT_FUNC(name, logType)                                                                                                                                                                  \
	template<typename... Args> static void name(std::string_view msg, Args... args) {                                                                                                                  \
		print<logType>(msg, std::forward<Args>(args)...);                                                                                                                                              \
	}

		FIF_PRINT_FUNC(info, LogType::INFO)
		FIF_PRINT_FUNC(warn, LogType::WARN)
		FIF_PRINT_FUNC(error, LogType::ERROR)
		FIF_PRINT_FUNC(debug, LogType::DEBUG)

	public:
		static std::vector<LoggerCallback> s_Callbacks;
	};
};// namespace fif::core
