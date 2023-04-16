#include "editorLayer.h"
#include "fif/core/application.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"
#include "fif/core/scopeTimer.h"
#include "fif/gfx/gfx.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"

#include "imgui.h"
#include "GLFW/glfw3.h"

#include <cmath>

void EditorLayer::update(float dt) {
}

void EditorLayer::render() {
	FIF_PROFILE_FUNC();
	float time = glfwGetTime();
	float cos = (std::cos(time) + 1.0f) * 0.5f;
	float sin = (std::sin(time) + 1.0f) * 0.5f;

	fif::gfx::Renderer2D::renderQuad({0.0f, 0.0f}, {100, 100}, 0.0f, {cos * 255, sin*255, 0, 255});
	fif::gfx::Renderer2D::renderQuad({0.0f, 120.0f}, {100,100}, glfwGetTime(), {sin*255, 0, cos*255, 255});
	fif::gfx::Renderer2D::renderQuad({0.0f, -120.0f}, {100,100}, -glfwGetTime(), {cos*255, 0, sin*255, 255});

	fif::gfx::Renderer2D::renderCircleFrag({-100.0f, 0.0f}, 100, {sin*255, cos*255, cos*255, 255});
	fif::gfx::Renderer2D::renderCircleTriangle({100.0f, 0.0f}, 100, m_CircleSegments, {sin*255, cos*255, sin, 255});
}

void EditorLayer::renderImGui() {
	if(ImGui::Begin("Performance")) {
		{
			const PerformanceStats &stats = fif::core::Application::getInstance().getLastFramePerformanceStats();
			ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
			ImGui::Text("FPS: %f", stats.fps);
		}

		if(ImGui::TreeNode("Profiler")) {
			const std::vector<fif::core::TimerResult> profilerResults = fif::core::Profiler::getResults();

			for(const fif::core::TimerResult &result : profilerResults)
				ImGui::Text("%s: %f ms", result.name.c_str(), result.durationMs);

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

		ImGui::SliderInt("Circle segments", &m_CircleSegments, 4, 100);
	}
	ImGui::End();
}
