#pragma once

#include <cstdint>

namespace fif::core {
	enum class EventType : std::uint8_t {
		None,
		WindowResized,
		WindowClosed,
		KeyPressed,
		KeyReleased,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled,
	};

	enum class EventCategory : std::uint8_t {
		Window = 1 << 0,
		Input = 1 << 1,
		Keyboard = (1 << 2) | Input,
		Mouse = (1 << 3) | Input,
	};
} // namespace fif::core
