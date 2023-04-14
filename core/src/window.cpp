#include "window.h"
#include "GLFW/glfw3.h"

fif::core::Window::Window(const std::string &title, const glm::i16vec2 &size) {
	m_GlfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
} 

bool fif::core::Window::shouldClose() const {
	return glfwWindowShouldClose(m_GlfwWindow);
}

void fif::core::Window::close() {
	glfwSetWindowShouldClose(m_GlfwWindow, true);
}
