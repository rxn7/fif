#pragma once

#include "event.hpp"

namespace fif::core {
	class KeyEvent : public Event {
	  public:
		EventCategory getCategory() const override { return EventCategory::Keyboard; }
		inline std::uint32_t getKeyCode() const { return m_KeyCode; }

	  protected:
		KeyEvent(std::uint32_t keyCode) : m_KeyCode(keyCode) {}

	  protected:
		std::uint32_t m_KeyCode;
	};

	class KeyPressedEvent final : public KeyEvent {
	  public:
		KeyPressedEvent(std::uint32_t keyCode) : KeyEvent(keyCode) {}

		FIF_EVENT_TYPE_DECL(EventType::KeyPressed);
	};

	class KeyReleasedEvent final : public KeyEvent {
	  public:
		KeyReleasedEvent(std::uint32_t keyCode) : KeyEvent(keyCode) {}

		FIF_EVENT_TYPE_DECL(EventType::KeyReleased);
	};
} // namespace fif::core
