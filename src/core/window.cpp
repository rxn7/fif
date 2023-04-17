#include "fif/core/window.h"
#include "fif/core/application.h"
#include "fif/core/assertion.h"

#include "fif/core/event/windowEvent.h"
#include "fif/core/opengl.h"

namespace fif::core {
	Window::Window(const WindowProperties &props) {
		glfwSetErrorCallback([]([[maybe_unused]] int error, const char *msg) {
			FIF_LOG_ERROR("GLFW Error: " << msg);
		});

		FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		mp_GlfwWindow = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(mp_GlfwWindow);

		glfwSetWindowUserPointer(mp_GlfwWindow, this);

		glfwSetWindowSizeCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int width, int height) {
			WindowResizeEvent event({width, height});
			Application::getInstance().onEvent(event);
		});

		glfwSetWindowCloseCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow) {
			WindowCloseEvent event;
			Application::getInstance().onEvent(event);
		});

		glfwSwapInterval(props.vsync);
	} 

	Window::~Window() {
		FIF_ASSERT(mp_GlfwWindow != nullptr, "Glfw window is not created!");
		glfwDestroyWindow(mp_GlfwWindow);
	}

	void Window::endFrame() {
		glfwSwapBuffers(mp_GlfwWindow);
		glfwPollEvents();
	}

	bool Window::getShouldClose() const {
		return glfwWindowShouldClose(mp_GlfwWindow);
	}

	void Window::setShouldClose(bool value) {
		glfwSetWindowShouldClose(mp_GlfwWindow, value);
	}
}
