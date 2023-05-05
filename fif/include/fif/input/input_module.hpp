#pragma once

#include "fif/core/module.hpp"
#include "fif/core/window.hpp"

namespace fif::input {
	class InputModule final : public core::Module {
	public:
		FIF_MODULE_NAME(Input)
		FIF_MODULE_INSTANCE_FUNC_DECL(InputModule);

		InputModule();

		glm::vec2 get_mouse_position();
		glm::vec2 get_last_mouse_position();

		inline bool is_key_held(i32 key) const { return m_Keys[key]; }
		inline bool is_button_held(i32 btn) const { return m_Buttons[btn]; }

	private:
		static constexpr i32 BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST + 1;
		static constexpr i32 KEY_COUNT = GLFW_KEY_LAST + 1;

		bool m_Buttons[BUTTON_COUNT];
		bool m_Keys[KEY_COUNT];

		glm::vec2 m_LastMousePosition;
		glm::vec2 m_MousePosition;
	};
}// namespace fif::input
