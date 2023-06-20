#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "grid.hpp"

#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/ecs/scene_serializer.hpp"
#include "fif/core/event/key_event.hpp"

#include <imgui.h>
#include <tinyfiledialogs.h>

#include <fstream>
#include <iterator>
#include <streambuf>

namespace fifed {
	FIF_MODULE_INSTANCE_IMPL(EditorModule);

	EditorModule::EditorModule() : m_IconManager("assets/textures/icons.png"), m_FrameBuffer({0, 0}), m_Grid(fif::gfx::GfxModule::get_instance()->get_renderer2D().get_camera(), m_FrameBuffer) {
		FIF_MODULE_INIT_INSTANCE();
		add_panel<ConsolePanel>();
	}

	EditorModule::~EditorModule() {
		ImGuiModule::get_instance()->delete_render_func(&EditorModule::on_render_im_gui);
	}

	void EditorModule::on_start() {
		mp_Application->set_pause(true);

		m_IconManager.add_icon(IconType::GITHUB, {{0.0f, 0.0f}, {230.0f, 225.0f}});
		m_IconManager.add_icon(IconType::LOGO, {{0.0f, 225.0f}, {48.0f, 48.0f}});
		m_IconManager.add_icon(IconType::PAUSE, {{48.0f, 225.0f}, {32.0f, 32.0f}});
		m_IconManager.add_icon(IconType::UNPAUSE, {{80.0f, 225.0f}, {32.0f, 32.0f}});

		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		static constexpr ImWchar ranges[] = {0x0020, 0x017f, 0};

		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18, nullptr, ranges);
		io.IniFilename = "layout.ini";

		mp_ViewportPanel = add_panel<ViewportPanel>(m_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>(mp_Application->get_scene());
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>(m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>(*mp_InspectorPanel);

		ImGuiModule::get_instance()->add_render_func(&EditorModule::on_render_im_gui);
	}

	void EditorModule::load_default_layout() {
		ImGui::LoadIniSettingsFromDisk("default_layout.ini");
	}

	void EditorModule::on_render_im_gui() {
		EditorModule *_this = EditorModule::get_instance();

		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::Button("About"))
				_this->m_AboutWindowOpen = true;

			if(ImGui::BeginMenu("Scene")) {
				if(ImGui::MenuItem("Save"))
					_this->save_scene();

				if(ImGui::MenuItem("Load"))
					_this->open_scene();

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Layout")) {
				if(ImGui::MenuItem("Load Default"))
					_this->load_default_layout();

				if(ImGui::MenuItem("Save"))
					ImGui::SaveIniSettingsToDisk("layout.ini");

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if(_this->m_AboutWindowOpen) {
			if(ImGui::Begin("About", &_this->m_AboutWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
				ImGui::Text("Fif v%u.%u.%u", 0, 0, 0);// TODO: version(major,minor,patch)
				if(ImGui::CollapsingHeader("License")) {
					static std::ifstream stream("LICENSE", std::ios::in | std::ios::binary);
					static std::string content(std::istreambuf_iterator<char>(stream), {});
					ImGui::TextWrapped(content.c_str());
				}
				if(_this->m_IconManager.imgui_button("Source", IconType::GITHUB)) {
#define GITHUB_URL "https://github.com/rxn7/fif"
#ifdef _WIN32
					system("start /b open " GITHUB_URL);
#elif defined(__linux__)
					system("xdg-open " GITHUB_URL "&");
#endif
				}
			}
			ImGui::End();
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

	void EditorModule::save_scene() {
		const char *filter = "*.yaml";
		char *path = tinyfd_saveFileDialog("Save scene", "scene.yaml", 1, &filter, "YAML file");
		if(path) {
			SceneSerializer serializer(mp_Application->get_scene());
			serializer.serialize(path);
			Logger::info("Scene saved to: %s", path);
		}
	}

	void EditorModule::open_scene() {
		std::stringstream workingDirectorySs;
		workingDirectorySs << std::filesystem::current_path() << "/";
		const std::string workingDirectoryStr = workingDirectorySs.str();
		const char *filter = "*.yaml";
		char *path = tinyfd_openFileDialog("Select scene", workingDirectoryStr.c_str(), 1, &filter, "YAML scene", false);
		if(path) {
			SceneSerializer serializer(mp_Application->get_scene());
			serializer.deserialize(path);
			Logger::info("Scene loaded: %s", path);
		}
	}

	void EditorModule::on_event(Event &event) {
		m_CameraController.on_event(event, mp_ViewportPanel->is_hovered());
		EventDispatcher::dispatch<KeyPressedEvent>(event, [&](KeyPressedEvent &keyEvent) {
			switch(keyEvent.get_key_code()) {
			// Go to selected entity
			case GLFW_KEY_F:
				if(TransformComponent *trans = mp_InspectorPanel->m_SelectedEntity.try_get_component<TransformComponent>()) {
					GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = trans->position;
					return true;
				}
				break;

			case GLFW_KEY_S:
				if(InputModule::get_instance()->is_modifier_held(GLFW_MOD_CONTROL))
					save_scene();
				break;

			case GLFW_KEY_O:
				if(InputModule::get_instance()->is_modifier_held(GLFW_MOD_CONTROL))
					open_scene();
				break;
			}

			return false;
		});
	}
}// namespace fifed
