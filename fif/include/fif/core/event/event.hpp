#pragma once

#include "fif/core/event/event_type.hpp"

#define FIF_EVENT_TYPE_DECL(type)                                                                                                                    \
	static EventType get_type_static() { return (type); }                                                                                            \
	EventType get_type() const override { return get_type_static(); }

namespace fif::core {
	class Event {
	public:
		virtual EventType get_type() const = 0;
		virtual EventCategory get_category() const = 0;
		inline bool is_hanlded() const { return m_Handled; }
		inline bool is_in_category(EventCategory category) const { return static_cast<int>(get_category()) & static_cast<int>(category); }

	public:
		bool m_Handled = false;
	};
}// namespace fif::core
