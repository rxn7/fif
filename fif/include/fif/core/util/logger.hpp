#pragma once

namespace fif::core {
	class Logger {
	public:
		enum class LogType : u8 { INFO, WARN, ERROR };

		using LoggerCallback = std::function<void(LogType, const char *msg)>;

		static void add_callback(LoggerCallback callback);

		template<typename... Args> static void info(const char *msg, Args... args) {
			size_t size = snprintf(NULL, 0, msg, std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			snprintf(buf, size, msg, std::forward<Args>(args)...);

			for(LoggerCallback &callback : s_Callbacks)
				callback(LogType::INFO, buf);

			std::printf("[INFO]\t");
			std::puts(buf);

			std::free(buf);
		}

		template<typename... Args> static void warn(const char *msg, Args... args) {
			size_t size = snprintf(NULL, 0, msg, std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			snprintf(buf, size, msg, std::forward<Args>(args)...);

			for(LoggerCallback &callback : s_Callbacks)
				callback(LogType::WARN, buf);

			std::printf("[WARN]\t");
			std::puts(buf);

			std::free(buf);
		}

		template<typename... Args> static void error(const char *msg, Args... args) {
			size_t size = snprintf(NULL, 0, msg, std::forward<Args>(args)...) + 1;

			char *buf = static_cast<char *>(std::malloc(size));
			snprintf(buf, size, msg, std::forward<Args>(args)...);

			for(auto &callback : s_Callbacks)
				callback(LogType::ERROR, buf);

			std::printf("[ERROR]\t");
			std::puts(buf);

			std::free(buf);
		}

	public:
		static std::vector<LoggerCallback> s_Callbacks;
	};
};// namespace fif::core
