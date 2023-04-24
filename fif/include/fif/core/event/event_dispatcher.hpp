#pragma once

#include "fif/core/event/event.hpp"

#include <functional>

namespace fif::core {
	class EventDispatcher {
	public:
		template<typename T, typename F> static bool dispatch(Event &event, const F &func) {
			static_assert(std::is_base_of<Event, T>().value, "T has to be derived from Event!");

			if(event.get_type() == T::get_type_static()) {
				event.m_Handled |= func(static_cast<T &>(event));
				return true;
			}

			return false;
		}
	};
}// namespace fif::core
