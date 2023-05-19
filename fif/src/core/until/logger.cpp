#define ANSI_COLOR(code) "\033[" #code "m"

namespace fif::core {
	std::vector<Logger::LoggerCallback> Logger::s_Callbacks;

	void Logger::add_callback(LoggerCallback callback) {
		s_Callbacks.push_back(callback);
	}

	void Logger::delete_callback(LoggerCallback callback) {
		const auto it = std::find(s_Callbacks.begin(), s_Callbacks.end(), callback);
		if(it == s_Callbacks.end()) {
			error("Cannot remove callback that doesn't exist in s_Callbacks");
			return;
		}

		s_Callbacks.erase(it);
	}

	template<> void Logger::print_prefix<Logger::LogType::INFO>() {
		std::printf(ANSI_COLOR(37) "[INFO] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::WARN>() {
		std::printf(ANSI_COLOR(33) "[WARN] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::ERROR>() {
		std::printf(ANSI_COLOR(31) "[ERROR] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::DEBUG>() {
		std::printf(ANSI_COLOR(32) "[DEBUG] ");
	}
}// namespace fif::core
