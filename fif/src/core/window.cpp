#include "fif/core/window.h"
#include "fif/core/application.h"
#include "fif/core/util/assertion.h"

#include "fif/core/event/keyEvent.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/event/windowEvent.h"
#include "fif/core/opengl.h"
#include "fif/core/profiler.h"
#include <GLFW/glfw3.h>

namespace fif::core {
	Window::Window(Application &app, const WindowProperties &props)
		: m_Size(props.size), m_App(app) {
		FIF_PROFILE_FUNC();

		glfwSetErrorCallback([]([[maybe_unused]] int error, const char *msg) {
			FIF_LOG_ERROR("GLFW Error: " << msg);
		});

		FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		mp_GlfwWindow =
			glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(mp_GlfwWindow);
		glfwSetWindowUserPointer(mp_GlfwWindow, this);

		glfwSetWindowSizeCallback(
			mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int width, int height) {
				Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
				WindowResizeEvent event({width, height});
				window->m_Size = glm::i32vec2(width, height);
				window->m_App.onEvent(event);
			});

		glfwSetWindowCloseCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow) {
			Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			WindowCloseEvent event;
			window->m_App.onEvent(event);
		});

		glfwSwapInterval(props.vsync);
	}

	Window::~Window() {
		FIF_ASSERT(mp_GlfwWindow != nullptr, "Glfw window is not created!");
		glfwDestroyWindow(mp_GlfwWindow);
	}

	void Window::endFrame() {
		FIF_PROFILE_FUNC();
		glfwSwapBuffers(mp_GlfwWindow);
		glfwPollEvents();
	}

	bool Window::getShouldClose() const {
		return glfwWindowShouldClose(mp_GlfwWindow);
	}

	void Window::setShouldClose(bool value) {
		glfwSetWindowShouldClose(mp_GlfwWindow, value);
	}
} // namespace fif::core
