#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "gfx_module.hpp"
#include "grid.hpp"

#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"

#include "fif/core/event/key_event.hpp"
#include "fif/gfx/components/transform_component.hpp"
#include "vertex_buffer_layout.hpp"

namespace fifed {
	FIF_MODULE_INSTANCE_IMPL(EditorModule);

	EditorModule::EditorModule() : m_FrameBuffer({0, 0}), m_Grid(fif::gfx::GfxModule::get_instance()->get_renderer2D().get_camera(), m_FrameBuffer) {
		FIF_MODULE_INIT_INSTANCE();
	}

	EditorModule::~EditorModule() {
		ImGuiModule::get_instance()->delete_render_func(&EditorModule::on_render_im_gui);
	}

	void EditorModule::on_start([[maybe_unused]] Application &app) {
		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18);
		io.IniFilename = "layout.ini";

		mp_ViewportPanel = add_panel<ViewportPanel>(m_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>();
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>(m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>(*mp_InspectorPanel);
		add_panel<ConsolePanel>();

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
		m_CameraController.update();
	}

	void EditorModule::pre_render() {
		m_FrameBuffer.start();
		m_Grid.render();
	}

	void EditorModule::on_render() {
		m_FrameBuffer.end();
	}

	void EditorModule::on_event(Event &event) {
		m_CameraController.on_event(event, mp_ViewportPanel->is_hovered());
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
