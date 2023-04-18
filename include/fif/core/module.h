#pragma once

#include "fif/core/event/event.h"

#include <string>

namespace fif::core {
	class Application;

	class Module {
	  public:
		virtual ~Module() {}

		virtual constexpr std::string_view getName() const = 0;
		virtual void onStart([[maybe_unused]] Application &app) {}
		virtual void onEvent([[maybe_unused]] Event &event) {}
		virtual void onUpdate() {}
		virtual void onRender() {}

	  protected:
		Module() {}
	};
} // namespace fif::core

#define FIF_MODULE_NAME(x)                                                                                                                                                                             \
	constexpr std::string_view getName() const override { return #x; }

#define FIF_MODULE_INIT_INSTANCE()                                                                                                                                                                     \
	FIF_ASSERT(s_Instance == nullptr, "There can only one instance of this module");                                                                                                                   \
	s_Instance = this;

#define FIF_MODULE_INSTANCE_FUNC_DECL(c) static c *getInstance();
#define FIF_MODULE_INSTANCE_IMPL(c)                                                                                                                                                                    \
	c *s_Instance = nullptr;                                                                                                                                                                           \
	c *c::getInstance() {                                                                                                                                                                              \
		FIF_ASSERT(s_Instance != nullptr, "There is no instance of " #c);                                                                                                                              \
		return s_Instance;                                                                                                                                                                             \
	}
