#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "common.hpp"
#include "grid.hpp"
#include "panels/entities/entities_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/settings/settings_panel.hpp"

#include "fif/core/ecs/scene.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/imgui/imgui_module.hpp"
#include "fif/input/input_module.hpp"

#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

EditorModule::EditorModule() {}
EditorModule::~EditorModule() {}

void EditorModule::on_start(Application &app) {
	mp_FrameBuffer = std::make_unique<FrameBuffer>(app.get_window().get_size());
	mp_LogoTexture = std::make_unique<Texture>("assets/logo.png", GL_NEAREST);

	ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));

	Grid::init();

	mp_ViewportPanel = add_panel<ViewportPanel>(*mp_FrameBuffer);
	add_panel<PerformancePanel>();
	add_panel<SettingsPanel>();
	add_panel<EntitiesPanel>();
}

void EditorModule::on_render_im_gui() {
	if(ImGuiModule::get_instance()->begin_dockspace()) {
		for(std::unique_ptr<EditorPanel> &panel : m_Panels)
			panel->render();
	}
	ImGui::End();
}

void EditorModule::on_update() {
	CameraController::update();

	mp_FrameBuffer->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	Grid::render();
}

void EditorModule::on_render() {
	mp_FrameBuffer->unbind();
}

void EditorModule::on_event(Event &event) {
	CameraController::on_event(event, mp_ViewportPanel->is_hovered());
}
