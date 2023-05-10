#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "grid.hpp"
#include "module.hpp"
#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "panels/status/status_panel.hpp"

namespace fifed {
	FIF_MODULE_INSTANCE_IMPL(EditorModule);

	EditorModule::EditorModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	EditorModule::~EditorModule() {}

	void EditorModule::on_start(Application &app) {
		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18);

		mp_FrameBuffer = std::make_unique<FrameBuffer>(app.get_window().get_size());

		mp_ViewportPanel = add_panel<ViewportPanel>(*mp_FrameBuffer);
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>();
		add_panel<StatusPanel>();
		InspectorPanel *inspector = add_panel<InspectorPanel>();
		add_panel<ScenePanel>(*inspector);
		add_panel<ConsolePanel>();

		Grid::init();

		ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));
	}

	void EditorModule::on_render_im_gui() {
		if(ImGuiModule::get_instance()->begin_dockspace())
			for(std::unique_ptr<EditorPanel> &panel : m_Panels)
				panel->render();

		ImGui::End();
	}

	void EditorModule::on_update() {
		CameraController::update();
	}

	void EditorModule::pre_render() {
		mp_FrameBuffer->start();
		Grid::render();
	}

	void EditorModule::on_render() {
		mp_FrameBuffer->end();
	}

	void EditorModule::on_event(Event &event) {
		CameraController::on_event(event, mp_ViewportPanel->is_hovered());
	}
}// namespace fifed
