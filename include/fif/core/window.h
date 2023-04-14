#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

namespace fif::core {
	struct WindowProperties {
		const std::string title; 
		const glm::i16vec2 size;
		bool vsync = true;
	};

	class Window {
	friend class Application;
	public:
		Window(const WindowProperties &props);
		~Window();
		bool shouldClose() const;
		void close();

		inline GLFWwindow *getGlfwWindow() const { return mp_GlfwWindow; }

	private:
		void startFrame();
		void endFrame();

	private:
		static void glfwErrorCallback(int error, const char *msg);
		static void glfwResizeCallback(GLFWwindow *window, int width, int height);

	private:
		GLFWwindow *mp_GlfwWindow = nullptr;
	};
}
