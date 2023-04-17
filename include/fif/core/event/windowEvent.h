#pragma once

#include "event.h"
#include "glm/vec2.hpp"

namespace fif::core {
	class WindowResizeEvent final : public Event {
	public:
		WindowResizeEvent(const glm::i32vec2 size) : m_Size(size) { }

		FIF_EVENT_TYPE_DECL(EventType::WindowResized);
		EventCategory getCategory() const override { return EventCategory::Window; }
		inline const glm::i32vec2 &getSize() const { return m_Size; }

	protected:
		glm::i32vec2 m_Size;
	};

	class WindowCloseEvent final : public Event {
	public:
		FIF_EVENT_TYPE_DECL(EventType::WindowClosed);
		EventCategory getCategory() const override { return EventCategory::Window; }
		inline const glm::i32vec2 &getSize() const { return m_Size; }

	protected:
		glm::i32vec2 m_Size;
	};
}
