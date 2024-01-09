#include "fif/core/window.hpp"
#include "fif/core/application.hpp"
#include "fif/core/event/key_event.hpp"
#include "fif/core/event/mouse_event.hpp"
#include "fif/core/event/window_event.hpp"

#include <stb_image.h>

namespace fif::core {
	Window::Window(Application &app, const WindowProperties &props) : m_Size(props.size), m_App(app) {
		glfwSetErrorCallback([]([[maybe_unused]] int error, const char *msg) { Logger::error("GLFW Error: %s", msg); });

		FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

		mp_GlfwWindow = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);
		FIF_ASSERT(mp_GlfwWindow != nullptr, "Failed to create GLFW window!")

		glfwMakeContextCurrent(mp_GlfwWindow);
		glfwSetWindowUserPointer(mp_GlfwWindow, this);

		glfwSetWindowSizeCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow, int width, int height) {
			Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			WindowResizeEvent event({width, height});
			window->m_Size = glm::i32vec2(width, height);
			window->m_App.on_event(event);
		});

		glfwSetWindowCloseCallback(mp_GlfwWindow, []([[maybe_unused]] GLFWwindow *glfwWindow) {
			Window *window = FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow);
			WindowCloseEvent event;
			window->m_App.on_event(event);
		});

		glfwSwapInterval(static_cast<i32>(props.vsync));

		if(!props.iconPath.empty())
			set_icon(props.iconPath);
	}

	Window::~Window() {
		FIF_ASSERT(mp_GlfwWindow != nullptr, "Glfw window is not created!");
		Logger::debug("GLFW destroyed");
		glfwDestroyWindow(mp_GlfwWindow);
		glfwTerminate();
	}

	void Window::end_frame() {
		glfwSwapBuffers(mp_GlfwWindow);
		glfwPollEvents();
	}

	void Window::close(const bool value) { glfwSetWindowShouldClose(mp_GlfwWindow, value); }

	void Window::set_icon(const std::string &path) {
		GLFWimage icon;
		icon.pixels = stbi_load(path.c_str(), &icon.width, &icon.height, nullptr, 4);

		if(icon.pixels)
			glfwSetWindowIcon(mp_GlfwWindow, 1, &icon);
		else
			Logger::error("Failed to open window icon '%s'", path.data());

		stbi_image_free(icon.pixels);
	}

	void Window::set_title(const std::string &title) { glfwSetWindowTitle(mp_GlfwWindow, title.c_str()); }
}// namespace fif::core
