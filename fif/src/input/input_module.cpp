#include "./lua_register.hpp"

#include "fif/core/event/key_event.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/input/input_module.hpp"

namespace fif::input {
	InputModule::InputModule() : m_EndFrameCallback(std::bind(&InputModule::end_frame, this)) {
		FIF_MODULE_INIT();

		core::Application &app = core::Application::get_instance();
		app.m_EndFrameHook.hook(m_EndFrameCallback);

		GLFWwindow *glfwWindow = app.get_window().get_glfw_window();

		glfwSetKeyCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(key >= 0 && key <= KEY_COUNT, "The key %d is out of range", key);

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				sp_Instance->m_State.keys[key] = true;
				core::KeyPressedEvent event(key, mods);
				window->get_application().on_event(event);
			} else if(action == GLFW_RELEASE) {
				sp_Instance->m_State.keys[key] = false;
				core::KeyReleasedEvent event(key, mods);
				window->get_application().on_event(event);
			}
		});

		glfwSetMouseButtonCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int button, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(button >= 0 && button <= BUTTON_COUNT, "The button %d is out of range", button);

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				sp_Instance->m_State.buttons[button] = true;
				core::MouseButtonPressedEvent event(button);
				window->get_application().on_event(event);
			} else if(action == GLFW_RELEASE) {
				sp_Instance->m_State.buttons[button] = false;
				core::MouseButtonReleasedEvent event(button);
				window->get_application().on_event(event);
			}
		});

		glfwSetScrollCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, double x, double y) {
			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			core::MouseScrolledEvent event({x, y});
			window->get_application().on_event(event);
		});

		glfwSetCursorPosCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, double x, double y) {
			InputModule &input = InputModule::get_instance();

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			input.m_MousePosition = {x, y};
			const vec2 delta = input.m_MousePosition - input.m_LastMousePosition;

			core::MouseMovedEvent event(input.m_MousePosition, delta);
			window->get_application().on_event(event);

			input.m_LastMousePosition = input.m_MousePosition;
		});

		register_lua_types(*this);
	}

	InputModule::~InputModule() { mp_Application->m_EndFrameHook.unhook(m_EndFrameCallback); }

	void InputModule::end_frame() { m_LastState = m_State; }

	vec2 InputModule::get_mouse_position() { return m_MousePosition; }

	vec2 InputModule::get_last_mouse_position() { return m_LastMousePosition; }
}// namespace fif::input
