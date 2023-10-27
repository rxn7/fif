#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class Application;

	class Module {
	public:
		virtual ~Module() {
		}
		virtual constexpr std::string_view get_name() const = 0;

	protected:
		Module() {
		}
		virtual void on_start() {
		}
		virtual void on_event([[maybe_unused]] Event &event) {
		}
		virtual void on_update() {
		}
		virtual void pre_render() {
		}
		virtual void on_render() {
		}

	protected:
		Application *mp_Application;
		friend class Application;
	};
}// namespace fif::core

#define FIF_MODULE(x)                                                                                                                                                                                  \
	FIF_MODULE_NAME_DECL(x)                                                                                                                                                                            \
	FIF_MODULE_INSTANCE_FUNC_DECL(x)                                                                                                                                                                   \
	FIF_MODULE_EXISTS_DECL(x)

#define FIF_MODULE_NAME_DECL(x)                                                                                                                                                                        \
	constexpr std::string_view get_name() const override {                                                                                                                                             \
		return #x;                                                                                                                                                                                     \
	}

#define FIF_MODULE_EXISTS_DECL(x)                                                                                                                                                                      \
	static bool exists() {                                                                                                                                                                             \
		return get_instance() != nullptr;                                                                                                                                                              \
	}

#define FIF_MODULE_INSTANCE_FUNC_DECL(c) static c *get_instance();
#define FIF_MODULE_INSTANCE_IMPL(c)                                                                                                                                                                    \
	c *s_Instance = nullptr;                                                                                                                                                                           \
	c *c::get_instance() {                                                                                                                                                                             \
		FIF_ASSERT(s_Instance != nullptr, "There is no instance of " #c);                                                                                                                              \
		return s_Instance;                                                                                                                                                                             \
	}

#define FIF_MODULE_INIT_INSTANCE()                                                                                                                                                                     \
	FIF_ASSERT(s_Instance == nullptr, "There can only one instance of this module");                                                                                                                   \
	s_Instance = this;
