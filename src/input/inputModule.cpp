#include "fif/input/inputModule.h"
#include "fif/core/application.h"
#include "fif/core/assertion.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/event/keyEvent.h"

#include "GLFW/glfw3.h"
#include "fif/core/module.h"
#include "fif/core/window.h"

namespace fif::input {
	static glm::vec2 s_LastMousePosition;
	FIF_MODULE_INSTANCE_IMPL(InputModule);

	InputModule::InputModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	void InputModule::onStart(core::Application &app) {
		GLFWwindow *glfwWindow = app.getWindow().getGlfwWindow();

		glfwSetKeyCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(key >= 0 && key <= KEY_COUNT, "The key " << key << " is out of range");

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				s_Instance->m_Keys[key] = true;
				core::KeyPressedEvent event(key);
				window->getApplication().onEvent(event);
			} else if (action == GLFW_RELEASE) {
				s_Instance->m_Keys[key] = false;
				core::KeyReleasedEvent event(key);
				window->getApplication().onEvent(event);
			}
		});

		glfwSetMouseButtonCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int button, int action, [[maybe_unused]] int mods) {
			FIF_ASSERT(button >= 0 && button <= BUTTON_COUNT, "The button " << button << " is out of range");

			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			if(action == GLFW_PRESS) {
				s_Instance->m_Buttons[button] = true;
				core::MouseButtonPressedEvent event(button);
				window->getApplication().onEvent(event);
			} else if (action == GLFW_RELEASE) {
				s_Instance->m_Buttons[button] = false;
				core::MouseButtonReleasedEvent event(button);
				window->getApplication().onEvent(event);
			}
		});

		glfwSetScrollCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, double x, double y) {
			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			core::MouseScrolledEvent event({x, y});
			window->getApplication().onEvent(event);
		});

		glfwSetCursorPosCallback(glfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, double x, double y) {
			core::Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			const glm::vec2 position(x,y);
			const glm::vec2 delta = position - s_LastMousePosition;

			core::MouseMovedEvent event(position, delta);
			s_LastMousePosition = position;
			window->getApplication().onEvent(event);
		});
	}
}
