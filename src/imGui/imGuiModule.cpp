#include "fif/imGui/imGuiModule.h"
#include "fif/core/application.h"
#include "fif/core/assertion.h"

#include "fif/core/event/event.h"
#include "fif/core/profiler.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <filesystem>

namespace fif::imgui {
	FIF_MODULE_INSTANCE_IMPL(ImGuiModule);

	ImGuiModule::ImGuiModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	ImGuiModule::~ImGuiModule() {
		FIF_PROFILE_FUNC();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void ImGuiModule::onStart(core::Application &app) {
		FIF_PROFILE_FUNC();
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplOpenGL3_Init("#version 300 es");
		ImGui_ImplGlfw_InitForOpenGL(app.getWindow().getGlfwWindow(), true);
	}

	void ImGuiModule::onUpdate([[maybe_unused]] float dt) {
		FIF_PROFILE_FUNC();
	}

	void ImGuiModule::onRender() {
		FIF_PROFILE_FUNC();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		for(const auto &renderFunc : m_RenderFunctions)
			renderFunc();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
