#pragma once

#include "fif/core/event/event.h"

#include <functional>

namespace fif::core {
	class EventDispatcher {
	public:
		EventDispatcher(Event &event) : m_Event(event) { }

		template<typename T, typename F>
		bool dispatch(const F &func) {
			static_assert(std::is_base_of<Event, T>().value, "T has to be derived from Event!");

			if(m_Event.getType() == T::getTypeStatic()) {
				m_Event.m_Handled |= func(static_cast<T&>(m_Event));
				return true;
			}

			return false;
		}

	private:
		Event &m_Event;
	};
}
