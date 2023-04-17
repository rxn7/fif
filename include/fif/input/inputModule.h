#pragma once

#include "fif/core/module.h"
#include "fif/core/window.h"
#include "fif/gfx/camera.h"

#include "fif/core/opengl.h"

namespace fif::input {
	class InputModule final : public core::Module {
	public:
		InputModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(InputModule);

		constexpr std::string_view getName() const override { return "Input"; }

		void onStart(core::Application &app) override;

		inline bool isKeyHeld(int key) const  { return m_Keys[key]; }
		inline bool isButtonHeld(int btn) const { return m_Buttons[btn]; }

	private:
		static constexpr std::int32_t BUTTON_COUNT = GLFW_MOUSE_BUTTON_LAST+1;
		static constexpr std::int32_t KEY_COUNT = GLFW_KEY_LAST+1;

		bool m_Buttons[BUTTON_COUNT];
		bool m_Keys[KEY_COUNT];
	};
}
