#pragma once

namespace fif::core {
	struct TagComponent final {
		TagComponent(const std::string &tag) : tag(tag) {}
		TagComponent(const char *tag) : tag(tag) {}

		std::string tag;
	};
}// namespace fif::core
