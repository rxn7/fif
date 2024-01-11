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
