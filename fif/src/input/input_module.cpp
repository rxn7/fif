#include "fif/input/input_module.hpp"
#include "fif/core/application.hpp"
#include "fif/core/event/key_event.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/module.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/window.hpp"

namespace fif {
	static glm::vec2 s_LastMousePosition;
	static glm::vec2 s_MousePosition;

	FIF_MODULE_INSTANCE_IMPL(InputModule);

	InputModule::InputModule() {
		FIF_MODULE_INIT_INSTANCE();

		GLFWwindow *glfwWindow = core::Application::get_instance().get_window().get_glfw_window();

		glfwSetKeyCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(key >= 0 && key <= KEY_COUNT, "The key " << key << " is out of range");

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				s_Instance->m_Keys[key] = true;
				core::KeyPressedEvent event(key);
				window->get_application().on_event(event);
			} else if(action == GLFW_RELEASE) {
				s_Instance->m_Keys[key] = false;
				core::KeyReleasedEvent event(key);
				window->get_application().on_event(event);
			}
		});

		glfwSetMouseButtonCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int button, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(button >= 0 && button <= BUTTON_COUNT, "The button " << button << " is out of range");

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				s_Instance->m_Buttons[button] = true;
				core::MouseButtonPressedEvent event(button);
				window->get_application().on_event(event);
			} else if(action == GLFW_RELEASE) {
				s_Instance->m_Buttons[button] = false;
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
			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			s_MousePosition = {x, y};
			const glm::vec2 delta = s_MousePosition - s_LastMousePosition;

			core::MouseMovedEvent event(s_MousePosition, delta);
			window->get_application().on_event(event);

			s_LastMousePosition = s_MousePosition;
		});
	}

	glm::vec2 InputModule::get_mouse_position() {
		return s_MousePosition;
	}

	glm::vec2 InputModule::get_last_mouse_position() {
		return s_LastMousePosition;
	}
}// namespace fif
