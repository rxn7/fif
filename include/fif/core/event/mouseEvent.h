#pragma once

#include "fif/core/event/event.h"
#include "fif/core/event/eventType.h"

#include "glm/vec2.hpp"

namespace fif::core {
	class MouseMovedEvent final : public Event {
	public:
		MouseMovedEvent(const glm::vec2 &position, const glm::vec2 &delta) : m_Position(position), m_Delta(delta) { }

		FIF_EVENT_TYPE_DECL(EventType::MouseMoved);
		EventCategory getCategory() const override { return EventCategory::Mouse; }
		inline const glm::vec2 &getPosition() const { return m_Position; }
		inline const glm::vec2 &getDelta() const { return m_Delta; }

	protected:
		glm::vec2 m_Position;
		glm::vec2 m_Delta;
	};

	class MouseScrolledEvent final : public Event {
	public:
		MouseScrolledEvent(const glm::vec2 &value) : m_Value(value) { }

		FIF_EVENT_TYPE_DECL(EventType::MouseScrolled);
		EventCategory getCategory() const override { return EventCategory::Mouse; }
		inline const glm::vec2 &getValue() const { return m_Value; }
		
	protected:
		glm::vec2 m_Value;
	};
	
	class MouseButtonEvent : public Event {
	public:
		inline std::uint32_t getButton() const { return m_Button; }
		EventCategory getCategory() const override { return EventCategory::Mouse; }

	protected:
		MouseButtonEvent(std::uint32_t btn) : m_Button(btn) { }

	protected:
		std::uint32_t m_Button;
	};
	
	class MouseButtonPressedEvent final : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(std::uint32_t btn) : MouseButtonEvent(btn) { }

		FIF_EVENT_TYPE_DECL(EventType::MouseButtonPressed);
	};
	
	class MouseButtonReleasedEvent final : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(std::uint32_t btn) : MouseButtonEvent(btn) { }

		FIF_EVENT_TYPE_DECL(EventType::MouseButtonReleased);
	};
}
