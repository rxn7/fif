#pragma once

#include "fif/core/types.hpp"

namespace fif::core {
	enum class EventType : u8 {
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

	enum class EventCategory : u8 {
		Window = 1 << 0,
		Input = 1 << 1,
		Keyboard = (1 << 2) | Input,
		Mouse = (1 << 3) | Input,
	};
} // namespace fif::core
