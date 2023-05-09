#pragma once

namespace fif::core {
	struct TagComponent final {
		TagComponent(const char *tag = "Entity") : tag(tag) {}

		std::string tag;
	};
}// namespace fif::core
