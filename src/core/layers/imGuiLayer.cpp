#include "fif/core/layers/imGuiLayer.h"
#include "GLFW/glfw3.h"
#include "fif/core/application.h"
#include "fif/core/performanceStats.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace fif::core {
	ImGuiLayer::ImGuiLayer() {
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(Application::getInstance().getWindow().getGlfwWindow(), true);
	}

	ImGuiLayer::~ImGuiLayer() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void ImGuiLayer::render() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if(ImGui::Begin("Performance")) {
			const PerformanceStats &stats = Application::getInstance().getLastFramePerformanceStats();
			ImGui::Text("Frame time: %f ms", stats.frameTimeMs);
			ImGui::Text("FPS: %f", stats.fps);
		}
		ImGui::End();

		if(ImGui::Begin("Options")) {
			static bool vsync = true;
			if(ImGui::Checkbox("VSync", &vsync))
				glfwSwapInterval(vsync);
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
