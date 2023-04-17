#pragma once

#include "fif/core/event/event.h"

#include <functional>

namespace fif::core {
	class EventDispatcher {
	public:
		template<typename T, typename F>
		static bool dispatch(Event &event, const F &func) {
			static_assert(std::is_base_of<Event, T>().value, "T has to be derived from Event!");

			if(event.getType() == T::getTypeStatic()) {
				event.m_Handled |= func(static_cast<T&>(event));
				return true;
			}

			return false;
		}
	};
}
