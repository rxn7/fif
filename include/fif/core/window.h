#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

namespace fif::core {
	struct WindowProperties final {
		const std::string title; 
		const glm::i16vec2 size;
		bool vsync = true;
	};

	class Window final {
	friend class Application;
	public:
		Window(const WindowProperties &props);
		~Window();
		bool getShouldClose() const;
		void setShouldClose(bool value);

		inline GLFWwindow *getGlfwWindow() const { return mp_GlfwWindow; }

	private:
		void endFrame();

	private:
		GLFWwindow *mp_GlfwWindow = nullptr;
	};
}
