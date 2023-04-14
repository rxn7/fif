#pragma once

#include "glm/vec2.hpp"
#include <string>

struct GLFWwindow;

namespace fif::core {
	class Window {
	public:
		Window(const std::string &title, const glm::i16vec2 &size);
		bool shouldClose() const;
		void close();

	private:
		GLFWwindow *m_GlfwWindow;
	};
}
