#define ANSI_COLOR(code) "\033[" #code "m"

namespace fif::core {
	template<> void Logger::print_prefix<Logger::LogType::Info>() {
		std::printf(ANSI_COLOR(37) "[INFO] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::Warn>() {
		std::printf(ANSI_COLOR(33) "[WARN] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::Error>() {
		std::printf(ANSI_COLOR(31) "[ERROR] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::Debug>() {
		std::printf(ANSI_COLOR(32) "[DEBUG] ");
	}
}// namespace fif::core
