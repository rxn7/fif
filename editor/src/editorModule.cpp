#include "editorModule.h"
#include "fif/core/application.h"
#include "fif/core/event/event.h"
#include "fif/core/event/eventDispatcher.h"
#include "fif/core/event/mouseEvent.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"
#include "fif/core/scopeTimer.h"
#include "fif/gfx/orthoCamera.h"
#include "fif/gfx/renderer2d.h"
#include "fif/imGui/imGuiModule.h"

#include "fif/input/inputModule.h"
#include "imgui.h"

#include <cmath>

EditorModule::EditorModule() {
}

EditorModule::~EditorModule() {
}

void EditorModule::onStart(fif::core::Application &app) {
	fif::imgui::ImGuiModule::getInstance()->addRenderFunc(std::bind(&EditorModule::onRenderImGui, this));
}

void EditorModule::onRender() {
	FIF_PROFILE_FUNC();
	float time = std::chrono::duration<float>(fif::core::Clock::now().time_since_epoch()).count();
	float cos = (std::cos(time) + 1.0f) * 0.5f;
	float sin = (std::sin(time) + 1.0f) * 0.5f;

	fif::gfx::Renderer2D::renderQuad({0.0f, 0.0f}, {100, 100}, 0.0f, {cos * 255, sin*255, 0, 255});
	fif::gfx::Renderer2D::renderQuad({0.0f, 120.0f}, {100,100}, time, {sin*255, 0, cos*255, 255});
	fif::gfx::Renderer2D::renderQuad({0.0f, -120.0f}, {100,100}, -time, {cos*255, 0, sin*255, 255});

	fif::gfx::Renderer2D::renderCircleFrag({-100.0f, 0.0f}, 100, {sin*255, cos*255, cos*255, 255});
	fif::gfx::Renderer2D::renderCircleTriangle({100.0f, 0.0f}, 100, 16, {sin*255, cos*255, sin, 255});
}

void EditorModule::onRenderImGui() {
	FIF_PROFILE_FUNC();
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
}

void EditorModule::onEvent(fif::core::Event &event) {
	FIF_PROFILE_FUNC();

	fif::core::EventDispatcher::dispatch<fif::core::MouseScrolledEvent>(event, [&](fif::core::MouseScrolledEvent &scrollEvent) {
		if(scrollEvent.isHanlded() || scrollEvent.getValue().y == 0)
			return false;

		fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::getCamera();
		cam.m_Size *= scrollEvent.getValue().y > 0 ? 0.9f : 1.1f;
		return true;
	});

	fif::core::EventDispatcher::dispatch<fif::core::MouseMovedEvent>(event, [&](fif::core::MouseMovedEvent &movedEvent) {
		if(!fif::input::InputModule::getInstance()->isButtonHeld(GLFW_MOUSE_BUTTON_LEFT))
			return false;

		fif::gfx::OrthoCamera &cam = fif::gfx::Renderer2D::getCamera();
		const float zoomFactor = cam.m_Size * 0.0025f;
		cam.m_Position.x -= movedEvent.getDelta().x * zoomFactor;
		cam.m_Position.y += movedEvent.getDelta().y * zoomFactor;
		return true;
	});
}
