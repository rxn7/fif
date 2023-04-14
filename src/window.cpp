#include "fif/window.h"
#include "fif/assertion.h"

#include "GLFW/glfw3.h"

fif::core::Window::Window(const WindowProperties &props) {
	glfwSetErrorCallback(glfwErrorCallback);
	FIF_ASSERT(glfwInit(), "Failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	m_GlfwWindow = glfwCreateWindow(props.size.x, props.size.y, props.title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(m_GlfwWindow);
} 

fif::core::Window::~Window() {
	FIF_ASSERT(m_GlfwWindow != nullptr, "Glfw window is not created!");
	close();
	glfwDestroyWindow(m_GlfwWindow);
}

void fif::core::Window::startFrame() {
}

void fif::core::Window::endFrame() {
	glfwSwapBuffers(m_GlfwWindow);
	glfwPollEvents();
}

bool fif::core::Window::shouldClose() const {
	return glfwWindowShouldClose(m_GlfwWindow);
}

void fif::core::Window::close() {
	glfwSetWindowShouldClose(m_GlfwWindow, true);
}

void fif::core::Window::glfwErrorCallback(int error, const char *msg) {
	// TODO: Logger
	std::cerr << "GLFW Error: " << msg << std::endl;
}
