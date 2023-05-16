#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "grid.hpp"

#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "panels/status/status_panel.hpp"

#include "fif/core/event/key_event.hpp"
#include "fif/gfx/components/transform_component.hpp"

namespace fifed {
	FIF_MODULE_INSTANCE_IMPL(EditorModule);

	EditorModule::EditorModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	EditorModule::~EditorModule() {
		ImGuiModule::get_instance()->delete_render_func(&EditorModule::on_render_im_gui);
	}

	void EditorModule::on_start(Application &app) {
		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18);
		io.IniFilename = "layout.ini";

		mp_FrameBuffer = std::make_unique<FrameBuffer>(app.get_window().get_size());

		mp_ViewportPanel = add_panel<ViewportPanel>(*mp_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>();
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>();
		add_panel<StatusPanel>();
		add_panel<ScenePanel>(*mp_InspectorPanel);
		add_panel<ConsolePanel>();

		Grid::init();

		ImGuiModule::get_instance()->add_render_func(&EditorModule::on_render_im_gui);
	}

	void EditorModule::load_default_layout() {
		ImGui::LoadIniSettingsFromDisk("default_layout.ini");
	}

	void EditorModule::on_render_im_gui() {
		EditorModule *_this = EditorModule::get_instance(); 

		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("Layout")) {
				if(ImGui::MenuItem("Load Default"))
					_this->load_default_layout();

				if(ImGui::MenuItem("Save"))
					ImGui::SaveIniSettingsToDisk("layout.ini");

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if(ImGuiModule::get_instance()->begin_dockspace())
			for(std::unique_ptr<EditorPanel> &panel : _this->m_Panels)
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
		EventDispatcher::dispatch<KeyPressedEvent>(event, [selectedEnt = mp_InspectorPanel->m_SelectedEntity](KeyPressedEvent &keyEvent) {
			switch(keyEvent.get_key_code()) {
			// Go to selected entity
			case GLFW_KEY_F:
				if(TransformComponent *trans = Application::get_instance()->get_scene().get_registry().try_get<TransformComponent>(selectedEnt)) {
					GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = trans->position;
					return true;
				}
				break;
			}

			return false;
		});
	}
}// namespace fifed
