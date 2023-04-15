#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/core/performanceStats.h"
#include "fif/gfx/gfx.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"

#include "imgui.h"
#include "GLFW/glfw3.h"

#include <cmath>

void EditorLayer::update(float dt) {
	fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::getCamera();
	cam.m_Position.x = std::cos(glfwGetTime() * 10) * 5;
	cam.m_Position.y = std::sin(glfwGetTime() * 10) * 5;
}

void EditorLayer::render() {
	fif::gfx::Renderer2D::renderQuad({0.0f, 0.0f}, {100, 100}, glfwGetTime(), {1.0f, 0.0f, 1.0f, 1.0f});
	fif::gfx::Renderer2D::renderCircle({0.0f, 0.0f}, 100, {1.0f, 1.0f, 0.0f, 1.0f});
	fif::gfx::Renderer2D::renderCircle({5.0f, 0.0f}, 25, {0.0f, 1.0f, 1.0f, 1.0f});
}

void EditorLayer::renderImGui() {
	if(ImGui::Begin("Performance")) {
		{
			const PerformanceStats &stats = fif::core::Application::getInstance().getLastFramePerformanceStats();
			ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
			ImGui::Text("FPS: %f", stats.fps);
		}
		if(ImGui::TreeNode("Renderer2D")) {
			const RendererStats &stats = fif::gfx::Renderer2D::getStats();
			ImGui::Text("Frame time: %f", stats.frameTimeMs);
			ImGui::Text("Primitives rendererd: %u", stats.primitivesRendered);
			ImGui::TreePop();
		}
	}
	ImGui::End();

	if(ImGui::Begin("Options")) {
		static bool vsync = true;
		if(ImGui::Checkbox("VSync", &vsync))
			glfwSwapInterval(vsync);

		if(ImGui::TreeNode("Camera")) {
			fif::gfx::OrthoCamera &camera = fif::gfx::Renderer2D::getCamera();
			ImGui::SliderFloat2("Position", &camera.m_Position[0], -1000.0f, 1000.0f);
			ImGui::SliderFloat("Size", &camera.m_Size, 0.1f, 1000.0f);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}
