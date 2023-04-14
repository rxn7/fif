#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

namespace fif::core {
	struct WindowProperties {
		const std::string title; 
		const glm::i16vec2 size;
	};

	class Window {
	public:
		Window(const WindowProperties &props);
		~Window();
		bool shouldClose() const;
		void close();

	friend class Application;
	private:
		void startFrame();
		void endFrame();

	private:
		static void glfwErrorCallback(int error, const char *msg);

	private:
		GLFWwindow *m_GlfwWindow = nullptr;
	};
}
