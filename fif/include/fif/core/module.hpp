#pragma once

#include "fif/core/event/event.hpp"
#include "fif/core/util/assertion.hpp"

#include <string>

namespace fif::core {
	class Application;

	class Module {
	public:
		virtual ~Module() {}

		virtual constexpr std::string_view get_name() const = 0;

		virtual void on_start([[maybe_unused]] Application &app) {}
		virtual void on_event([[maybe_unused]] Event &event) {}
		virtual void on_update() {}
		virtual void on_render() {}

	protected:
		Module() {}
	};
}// namespace fif::core

#define FIF_MODULE_NAME(x)                                                                                                                           \
	constexpr std::string_view get_name() const override { return #x; }

#define FIF_MODULE_INIT_INSTANCE()                                                                                                                   \
	FIF_ASSERT(s_Instance == nullptr, "There can only one instance of this module");                                                                 \
	s_Instance = this;

#define FIF_MODULE_INSTANCE_FUNC_DECL(c) static c *get_instance();
#define FIF_MODULE_INSTANCE_IMPL(c)                                                                                                                  \
	c *s_Instance = nullptr;                                                                                                                         \
	c *c::get_instance() {                                                                                                                           \
		FIF_ASSERT(s_Instance != nullptr, "There is no instance of " #c);                                                                            \
		return s_Instance;                                                                                                                           \
	}
