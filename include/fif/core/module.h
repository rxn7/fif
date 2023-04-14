#pragma once

#include <functional>

namespace fif::core {
	struct Module {
		std::function<void()> initFunc;
		std::function<void(float)> updateFunc;
		std::function<void()> renderFunc;
	};
}
