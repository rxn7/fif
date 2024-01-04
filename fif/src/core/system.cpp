#include "fif/core/system.hpp"

namespace fif::core {
	void System::open_url(const std::string &url) {
#ifdef _WIN32
		system(("start /b open " + url).c_str());
#elif defined(__linux__)
		system(("xdg-open " + url + "&").c_str());
#else
		Logger::error("fif::core::System::open_url() is not supported on this system!");
#endif
	}
}// namespace fif::core
