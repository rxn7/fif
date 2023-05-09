namespace fif::core {
	std::vector<Logger::LoggerCallback> Logger::s_Callbacks;

	void Logger::add_callback(LoggerCallback callback) {
		s_Callbacks.push_back(callback);
	}
}// namespace fif::core
