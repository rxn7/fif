#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "components/sprite_component.hpp"
#include "components/transform_component.hpp"
#include "grid.hpp"
#include "panels/entities/entities_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "panels/status/status_panel.hpp"

namespace fifed {
	EditorModule::EditorModule() {}
	EditorModule::~EditorModule() {}

	void EditorModule::on_start(Application &app) {
		mp_FrameBuffer = std::make_unique<FrameBuffer>(app.get_window().get_size());

		mp_ViewportPanel = add_panel<ViewportPanel>(*mp_FrameBuffer);
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>();
		add_panel<StatusPanel>();
		InspectorPanel *inspector = add_panel<InspectorPanel>();
		add_panel<EntitiesPanel>(*inspector);

		Grid::init();

		ImGuiModule::get_instance()->add_render_func(std::bind(&EditorModule::on_render_im_gui, this));
	}// namespace fifed

	void EditorModule::on_render_im_gui() {
		if(ImGuiModule::get_instance()->begin_dockspace())
			for(std::unique_ptr<EditorPanel> &panel : m_Panels)
				panel->render();

		ImGui::End();
	}

	void EditorModule::on_update() {
		mp_FrameBuffer->start();
		CameraController::update();
		Grid::render();
	}

	void EditorModule::on_render() {
		mp_FrameBuffer->end();
	}

	void EditorModule::on_event(Event &event) {
		CameraController::on_event(event, mp_ViewportPanel->is_hovered());
	}
}// namespace fifed
