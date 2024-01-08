#pragma once

#include "fif/core/event/event_type.hpp"

namespace fif::core {
#define FIF_EVENT(T)                                                                                                                                                                                   \
	static inline constexpr std::string_view get_name() { return #T; }                                                                                                                                 \
	static inline EventType get_type_static() { return (T); }                                                                                                                                          \
	inline EventType get_type() const override { return get_type_static(); }

	class Event {
	public:
		virtual EventType get_type() const = 0;
		virtual EventCategory get_category() const = 0;

		inline bool is_hanlded() const { return m_Handled; }

	public:
		bool m_Handled = false;
	};
}// namespace fif::core
