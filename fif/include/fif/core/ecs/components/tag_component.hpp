#pragma once

namespace fif::core {
	struct TagComponent : Component {
		TagComponent(const std::string &tag) : tag(tag) {}
		TagComponent(const char *tag) : tag(tag) {}

		std::string tag;
	};
}// namespace fif::core
