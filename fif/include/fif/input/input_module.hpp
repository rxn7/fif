#pragma once

#include "fif/core/module.hpp"
#include "fif/core/window.hpp"

namespace fif::input {
	constexpr i32 BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;
	constexpr i32 KEY_COUNT = GLFW_KEY_LAST + 1;

	struct InputState final {
		InputState() {
			reset();
		}

		InputState &operator=(const InputState &other) {
			for(u32 i = 0; i < BUTTON_COUNT; ++i)
				buttons[i] = other.buttons[i];

			for(u32 i = 0; i < KEY_COUNT; ++i)
				keys[i] = other.keys[i];

			return *this;
		}

		void reset() {
			for(bool &btn : buttons)
				btn = false;

			for(bool &key : keys)
				key = false;
		}

		std::array<bool, BUTTON_COUNT> buttons;
		std::array<bool, KEY_COUNT> keys;
	};

	class InputModule final : public core::Module {
	public:
		FIF_MODULE(InputModule)

		InputModule();
		~InputModule();

		vec2 get_mouse_position();
		vec2 get_last_mouse_position();

		inline bool is_key_held(i32 key) const {
			return m_State.keys[key];
		}

		inline bool is_button_held(i32 btn) const {
			return m_State.buttons[btn];
		}

		inline bool is_button_just_pressed(i32 btn) const {
			return m_State.buttons[btn] && !m_LastState.buttons[btn];
		}

		inline bool is_key_just_pressed(i32 key) const {
			return m_State.keys[key] && !m_LastState.keys[key];
		}

	private:
		void end_frame();

	private:
		Callback<> m_EndFrameCallback;

		InputState m_State;
		InputState m_LastState;

		vec2 m_LastMousePosition;
		vec2 m_MousePosition;
	};
}// namespace fif::input
