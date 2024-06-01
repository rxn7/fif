#include "fif/core/system.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

namespace fif::core {
	void System::open_url(const std::string &url) {
		Logger::debug("Opening url: %s", url.c_str());
#ifdef _WIN32
		ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWDEFAULT);
#elif defined(__linux__)
		system(std::format("xdg-open {} &", url).c_str());
#else
		Logger::error("fif::core::System::open_url() is not supported on this system!");
#endif
	}

	void System::open_file(const std::filesystem::path &path) {
		Logger::debug("Opening file: %s", path.string().c_str());
#ifdef _WIN32
		ShellExecuteA(NULL, "open", path.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
#elif defined(__linux__)
		system(std::format("xdg-open {} &", path.string()).c_str());
#else
		Logger::error("fif::core::System::open_file() is not supported on this system!");
#endif
	}

	std::string System::camel_case_to_snake_case(const std::string &camelCase) {
		std::ostringstream ss;

		for(u8 i = 0; i < camelCase.size(); ++i) {
			const char c = camelCase[i];

			if(std::isupper(c)) {
				if(i > 0 && std::islower(camelCase[i - 1])) {
					ss << '_';
				}

				ss << static_cast<char>(std::tolower(c));
				continue;
			}

			ss << c;
		}

		return ss.str();
	}
}// namespace fif::core
