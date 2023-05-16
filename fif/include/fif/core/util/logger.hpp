#pragma once

namespace fif::core {
	class Logger {
	public:
		enum class LogType : u8 { INFO, WARN, ERROR, DEBUG };

		typedef void (*LoggerCallback)(LogType, const char *msg);

		static void add_callback(LoggerCallback callback);
		static void delete_callback(LoggerCallback callback);

		template<LogType> static void print_prefix();

		template<LogType T, typename... Args> static void print(const char *msg, Args... args) {
			print_prefix<T>();

			size_t size = snprintf(NULL, 0, msg, std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			snprintf(buf, size, msg, std::forward<Args>(args)...);

			std::puts(buf);

			for(LoggerCallback &callback : s_Callbacks)
				callback(T, buf);

			std::free(buf);
		}

		template<typename... Args> static void info(const char *msg, Args... args) { print<LogType::INFO>(msg, std::forward<Args>(args)...); }
		template<typename... Args> static void warn(const char *msg, Args... args) { print<LogType::WARN>(msg, std::forward<Args>(args)...); }
		template<typename... Args> static void error(const char *msg, Args... args) { print<LogType::ERROR>(msg, std::forward<Args>(args)...); }
		template<typename... Args> static void debug(const char *msg, Args... args) { print<LogType::DEBUG>(msg, std::forward<Args>(args)...); }

	public:
		static std::vector<LoggerCallback> s_Callbacks;
	};
};// namespace fif::core
