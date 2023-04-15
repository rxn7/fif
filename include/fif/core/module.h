#pragma once

#include <functional>
#include <string>

namespace fif::core {
	struct Module {
		const std::string name;
		std::function<void()> initFunc;
		std::function<void(float)> updateFunc;
		std::function<void()> renderFunc;
	};
}
