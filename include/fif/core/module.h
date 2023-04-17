#pragma once

#include "fif/core/event/event.h"

#include <functional>
#include <string>

namespace fif::core {
	class Application;

	class Module {
		public:
			Module() { }
			virtual ~Module() { }

			virtual constexpr std::string_view getName() const = 0;
			virtual void onAttach(Application &app) = 0;
			virtual void onDetach() = 0;
			virtual void onEvent(Event &event) = 0;
			virtual void update(float dt) = 0;
			virtual void render() = 0;
	};
}
