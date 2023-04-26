#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

#define FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow) reinterpret_cast<fif::core::Window *>(glfwGetWindowUserPointer(glfwWindow))

namespace fif::core {
	class Application;

	struct WindowProperties final {
		const std::string title;
		const glm::i16vec2 size;
		bool vsync = true;
	};

	class Window final {
		friend class Application;

	public:
		Window(Application &app, const WindowProperties &props);
		~Window();

		inline glm::i16vec2 get_size() const { return m_Size; }
		bool get_should_close() const;
		void set_should_close(bool value);

		inline GLFWwindow *get_glfw_window() const { return mp_GlfwWindow; }
		inline Application &get_application() const { return m_App; }

	private:
		void end_frame();

	private:
		glm::i16vec2 m_Size;
		Application &m_App;
		GLFWwindow *mp_GlfwWindow = nullptr;
	};
}// namespace fif::core