#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

#define FIF_GET_WINDOW_FROM_GLFW_WINDOW(glfwWindow) reinterpret_cast<fif::core::Window*>(glfwGetWindowUserPointer(glfwWindow))

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

		inline glm::i16vec2 getSize() const { return m_Size; }
		bool getShouldClose() const;
		void setShouldClose(bool value);

		inline GLFWwindow *getGlfwWindow() const { return mp_GlfwWindow; }
		inline Application &getApplication() const { return m_App; }

	private:
		void endFrame();

	private:
		glm::i16vec2 m_Size;
		Application &m_App;
		GLFWwindow *mp_GlfwWindow = nullptr;
	};
}
