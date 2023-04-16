#include "fif/core/window.h"
#include "GLFW/glfw3.h"
#include "fif/core/assertion.h"

#include "fif/core/opengl.h"

namespace fif::core {
	Window::Window(const WindowProperties &props) {
		glfwSetErrorCallback(glfwErrorCallback);
		FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		mp_GlfwWindow = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);

		glfwMakeContextCurrent(mp_GlfwWindow);
		glfwSetWindowSizeCallback(mp_GlfwWindow, glfwResizeCallback);

		glfwSwapInterval(props.vsync);
	} 

	Window::~Window() {
		FIF_ASSERT(mp_GlfwWindow != nullptr, "Glfw window is not created!");
		close();
		glfwDestroyWindow(mp_GlfwWindow);
	}

	void Window::startFrame() {
	}

	void Window::endFrame() {
		glfwSwapBuffers(mp_GlfwWindow);
		glfwPollEvents();
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(mp_GlfwWindow);
	}

	void Window::close() {
		glfwSetWindowShouldClose(mp_GlfwWindow, true);
	}

	void Window::glfwErrorCallback(int error, const char *msg) {
		// TODO: Logger
		std::cerr << "GLFW Error: " << msg << std::endl;
	}

	void Window::glfwResizeCallback(GLFWwindow *window, int width, int height) {
		glViewport(0, 0, width, height);
	}
}
