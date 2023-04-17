#pragma once

#include "fif/core/event/event.h"

#include <functional>
#include <string>

namespace fif::core {
	class Application;

	class Module {
		public:
			virtual ~Module() { }

			virtual constexpr std::string_view getName() const = 0;
			virtual void onAttach(Application &app) = 0;
			virtual void onDetach() = 0;
			virtual void onEvent(Event &event) = 0;
			virtual void update(float dt) = 0;
			virtual void render() = 0;

		protected:
			Module() { }
	};
}

#define FIF_MODULE_INIT_INSTANCE() \
	FIF_ASSERT(s_Instance == nullptr, "There can only one instance of this module"); \
	s_Instance = this;

#define FIF_MODULE_INSTANCE_FUNC_DECL(c) static c *getInstance();
#define FIF_MODULE_INSTANCE_IMPL(c) \
	c *s_Instance = nullptr; \
	c *c::getInstance() { \
		FIF_ASSERT(s_Instance != nullptr, "There is no instance of " #c); \
		return s_Instance;  \
	}
