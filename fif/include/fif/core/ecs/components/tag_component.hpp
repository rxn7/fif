#pragma once

namespace fif::core {
	struct TagComponent final {
		TagComponent(const std::string &_tag = "Entity") : tag(_tag) {}
		std::string tag;
	};
}// namespace fif::core
