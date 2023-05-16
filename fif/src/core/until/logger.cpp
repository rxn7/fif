namespace fif::core {
	std::vector<Logger::LoggerCallback> Logger::s_Callbacks;

	void Logger::add_callback(LoggerCallback callback) {
		s_Callbacks.push_back(callback);
	}

	template<> void Logger::print_prefix<Logger::LogType::INFO>() {
		std::printf("[INFO] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::WARN>() {
		std::printf("[WARN] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::ERROR>() {
		std::printf("[ERROR] ");
	}
	template<> void Logger::print_prefix<Logger::LogType::DEBUG>() {
		std::printf("[DEBUG] ");
	}
}// namespace fif::core
