#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class Application;

	class Module {
	public:
		virtual ~Module() = default;

		virtual constexpr std::string_view get_name() const = 0;

		Application *get_application() const { return mp_Application; }

	protected:
		Module() {}

	protected:
		Application *mp_Application;
		friend class Application;
	};
}// namespace fif::core

#define FIF_MODULE(T)                                                                                                                                                                                  \
	constexpr inline std::string_view get_name() const override { return #T; }                                                                                                                         \
	static inline T *sp_Instance;                                                                                                                                                                      \
	static inline T &get_instance() {                                                                                                                                                                  \
		FIF_ASSERT(sp_Instance != nullptr, "There is no instance of " #T);                                                                                                                             \
		return *sp_Instance;                                                                                                                                                                           \
	}                                                                                                                                                                                                  \
	static inline bool exists() { return sp_Instance != nullptr; }

#define FIF_MODULE_INIT()                                                                                                                                                                              \
	FIF_ASSERT(sp_Instance == nullptr, "There can only one instance of this module");                                                                                                                  \
	sp_Instance = this;                                                                                                                                                                                \
	fif::core::Logger::debug("Module '%s' started", get_name().data());
