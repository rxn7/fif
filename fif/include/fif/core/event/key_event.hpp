#pragma once

#include "fif/core/event/event.hpp"

namespace fif::core {
	class KeyEvent : public Event {
	public:
		KeyEvent(i32 keyCode, i32 modifierBits) : m_KeyCode(keyCode), m_ModifierBits(modifierBits) {}

		EventCategory get_category() const override { return EventCategory::Keyboard; }

		inline i32 get_key_code() const { return m_KeyCode; }

		inline i32 get_modifier_bits() const { return m_ModifierBits; }

	protected:
		i32 m_KeyCode;
		i32 m_ModifierBits;
	};

	class KeyPressedEvent final : public KeyEvent {
	public:
		using KeyEvent::KeyEvent;
		FIF_EVENT(EventType::KeyPressed);
	};

	class KeyReleasedEvent final : public KeyEvent {
	public:
		using KeyEvent::KeyEvent;
		FIF_EVENT(EventType::KeyReleased);
	};
}// namespace fif::core
