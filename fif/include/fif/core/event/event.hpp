#pragma once

#include "fif/core/event/eventType.hpp"

#define FIF_EVENT_TYPE_DECL(type)                                                                  \
	static EventType getTypeStatic() { return (type); }                                            \
	EventType getType() const override { return getTypeStatic(); }

namespace fif::core {
	class Event {
		friend class EventDispatcher;

	  public:
		virtual EventType getType() const = 0;
		virtual EventCategory getCategory() const = 0;
		inline bool isHanlded() const { return m_Handled; }

	  protected:
		bool m_Handled = false;
	};
} // namespace fif::core
