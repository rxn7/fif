#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/core/performanceStats.h"
#include "fif/gfx/gfx.h"
#include "fif/gfx/renderer2d.h"

#include "imgui.h"
#include "GLFW/glfw3.h"

#include <cmath>

static float x = 0.0f;

void EditorLayer::update(float dt) {
	x += dt;
}

void EditorLayer::render() {
	fif::gfx::Renderer2D::renderQuad({std::cos(x),std::sin(x)}, {100, 100}, {1.0f, 0.0f, 1.0f, 1.0f});
	fif::gfx::Renderer2D::renderCircle({std::sin(x),std::cos(x)}, 50, {1.0f, 1.0f, 0.0f, 1.0f});
}

void EditorLayer::renderImGui() {
	if(ImGui::Begin("Performance")) {
		const PerformanceStats &stats = fif::core::Application::getInstance().getLastFramePerformanceStats();
		ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
		ImGui::Text("FPS: %f", stats.fps);
	}
	ImGui::End();

	if(ImGui::Begin("Options")) {
		static bool vsync = true;
		if(ImGui::Checkbox("VSync", &vsync))
			glfwSwapInterval(vsync);

		if(ImGui::Begin("Camera")) {
			fif::gfx::Camera &camera = fif::gfx::getCamera();
			ImGui::SliderFloat3("Position", &camera.m_Position[0], -100.0f, 100.0f);
			ImGui::SliderFloat3("Direction", &camera.m_Direction[0], -1.0f, 1.0f);
			ImGui::Checkbox("Ortho", &camera.m_Ortho);
			ImGui::SliderFloat("Fov", &camera.m_Fov, 1.0f, 100.0f);
			ImGui::SliderFloat("Z Near", &camera.m_ZNear, 0.0f, 1.0f);
			ImGui::SliderFloat("Z Far", &camera.m_ZFar, 1.0f, 100.0f);
		}
		ImGui::End();
	}
	ImGui::End();
}
