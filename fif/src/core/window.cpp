#include "fif/core/window.hpp"
#include "fif/core/application.hpp"
#include "fif/core/event/keyEvent.hpp"
#include "fif/core/event/mouseEvent.hpp"
#include "fif/core/event/windowEvent.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/assertion.hpp"

namespace fif::core {
	Window::Window(Application &app, const WindowProperties &props) : m_Size(props.size), m_App(app) {
		FIF_PROFILE_FUNC();

		glfwSetErrorCallback([]([[maybe_unused]] int error, const char *msg) { FIF_LOG_ERROR("GLFW Error: " << msg); });

		FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

		mp_GlfwWindow = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(mp_GlfwWindow);
		glfwSetWindowUserPointer(mp_GlfwWindow, this);

		glfwSetWindowSizeCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int width, int height) {
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

		glfwSwapInterval(static_cast<int>(props.vsync));
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
		return glfwWindowShouldClose(mp_GlfwWindow) != 0;
	}

	void Window::setShouldClose(bool value) {
		glfwSetWindowShouldClose(mp_GlfwWindow, static_cast<int>(value));
	}
} // namespace fif::core
