#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class MouseMovedEvent final : public Event {
	public:
		MouseMovedEvent(const vec2 &position, const vec2 &delta) : m_Position(position), m_Delta(delta) {}

		FIF_EVENT_TYPE_DECL(EventType::MouseMoved);
		EventCategory get_category() const override { return EventCategory::Mouse; }
		inline const vec2 &get_position() const { return m_Position; }
		inline const vec2 &get_delta() const { return m_Delta; }

	protected:
		vec2 m_Position;
		vec2 m_Delta;
	};

	class MouseScrolledEvent final : public Event {
	public:
		MouseScrolledEvent(const vec2 &value) : m_Value(value) {}

		FIF_EVENT_TYPE_DECL(EventType::MouseScrolled);
		EventCategory get_category() const override { return EventCategory::Mouse; }
		inline const vec2 &get_value() const { return m_Value; }

	protected:
		vec2 m_Value;
	};

	class MouseButtonEvent : public Event {
	public:
		inline u32 get_button() const { return m_Button; }
		EventCategory get_category() const override { return EventCategory::Mouse; }

	protected:
		MouseButtonEvent(u32 btn) : m_Button(btn) {}

	protected:
		u32 m_Button;
	};

	class MouseButtonPressedEvent final : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(u32 btn) : MouseButtonEvent(btn) {}

		FIF_EVENT_TYPE_DECL(EventType::MouseButtonPressed);
	};

	class MouseButtonReleasedEvent final : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(u32 btn) : MouseButtonEvent(btn) {}

		FIF_EVENT_TYPE_DECL(EventType::MouseButtonReleased);
	};
}// namespace fif::core
