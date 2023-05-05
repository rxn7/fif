#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class KeyEvent : public Event {
	public:
		EventCategory get_category() const override { return EventCategory::Keyboard; }
		inline u32 get_key_code() const { return m_KeyCode; }

	protected:
		KeyEvent(u32 keyCode) : m_KeyCode(keyCode) {}

	protected:
		u32 m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent {
	public:
		KeyPressedEvent(u32 keyCode) : KeyEvent(keyCode) {}

		FIF_EVENT_TYPE_DECL(EventType::KeyPressed);
	};

	class KeyReleasedEvent final : public KeyEvent {
	public:
		KeyReleasedEvent(u32 keyCode) : KeyEvent(keyCode) {}

		FIF_EVENT_TYPE_DECL(EventType::KeyReleased);
	};
}// namespace fif::core
