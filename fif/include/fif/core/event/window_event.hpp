#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class WindowResizeEvent final : public Event {
	public:
		WindowResizeEvent(const i32vec2 size) : m_Size(size) {}
		FIF_EVENT(EventType::WindowResized);

		EventCategory get_category() const override { return EventCategory::Window; }
		inline const i32vec2 &get_size() const { return m_Size; }

	protected:
		i32vec2 m_Size;
	};

	class WindowCloseEvent final : public Event {
	public:
		FIF_EVENT(EventType::WindowClosed);
		EventCategory get_category() const override { return EventCategory::Window; }
		inline const i32vec2 &get_size() const { return m_Size; }

	protected:
		i32vec2 m_Size;
	};
}// namespace fif::core
