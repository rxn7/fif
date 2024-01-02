#include "editor_module.hpp"
#include "camera_controller.hpp"
#include "fif/gfx/gfx_module.hpp"
#include "grid.hpp"

#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/event/key_event.hpp"
#include "fif/core/serialization/project_serializer.hpp"
#include "fif/core/serialization/scene_serializer.hpp"

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
		if(m_ProjectLoaded) {
			ImGuiModule::get_instance()->delete_render_func(&EditorModule::render_editor_imgui);
		}
	}

	void EditorModule::on_start() {
		set_play_mode(false);

		static constexpr ImWchar ranges[] = {0x0020, 0x017f, 0};
		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18, nullptr, ranges);
		io.IniFilename = "layout.ini";

		ImGuiModule::get_instance()->add_render_func(&EditorModule::render_project_manager_imgui);
	}

	void EditorModule::load_default_layout() {
		ImGui::LoadIniSettingsFromDisk("default_layout.ini");
	}

	void EditorModule::render_project_manager_imgui() {
		EditorModule *_this = EditorModule::get_instance();

		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		if(ImGui::Begin("Project Manager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize)) {
			constexpr ImVec2 BUTTON_SIZE{200, 100};

			ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - BUTTON_SIZE.x);
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.5f - BUTTON_SIZE.y);

			if(ImGui::Button("Create new project", BUTTON_SIZE)) {
				_this->create_project_dialog();
			}

			ImGui::SameLine();

			if(ImGui::Button("Open project", BUTTON_SIZE)) {
				_this->open_project_dialog();
			}

			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	void EditorModule::render_editor_imgui() {
		EditorModule *_this = EditorModule::get_instance();

		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::Button("About"))
				_this->m_AboutWindowOpen = true;

			if(ImGui::BeginMenu("Scene")) {
				if(ImGui::MenuItem("Save", "Ctrl+S"))
					_this->save_scene();

				if(ImGui::MenuItem("Load", "Ctrl+O"))
					_this->open_scene_dialog();

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Layout")) {
				if(ImGui::MenuItem("Load Default"))
					_this->load_default_layout();

				if(ImGui::MenuItem("Save"))
					ImGui::SaveIniSettingsToDisk("layout.ini");

				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Shortcuts"))
				_this->m_ShortcutsWindowOpen = true;

			ImGui::EndMainMenuBar();
		}

		if(_this->m_AboutWindowOpen) {
			ImGui::SetNextWindowSize(ImVec2(350, 200));
			if(ImGui::Begin("About", &_this->m_AboutWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
				ImGui::Text("Fif v%u.%u.%u", 0, 0, 0);// TODO: version(major,minor,patch)
				if(ImGui::CollapsingHeader("License")) {
					static std::ifstream stream("LICENSE", std::ios::in | std::ios::binary);
					static std::string content(std::istreambuf_iterator<char>(stream), {});
					ImGui::TextWrapped("%s", content.c_str());
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

		if(_this->m_ShortcutsWindowOpen) {
			ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);
			// TODO: scrollbar here
			if(ImGui::Begin("Shortcuts", &_this->m_ShortcutsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
				for(const Shortcut &shortcut : _this->m_Shortcuts)
					ImGui::TextWrapped("%s", shortcut.get_description().c_str());

			ImGui::End();
		}

		if(ImGuiModule::get_instance()->begin_dockspace())
			for(std::shared_ptr<EditorPanel> &panel : _this->m_Panels)
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

	bool EditorModule::create_project_dialog() {
		const char *fileDialogResult = tinyfd_selectFolderDialog("Select directory", NULL);

		if(!fileDialogResult)
			return false;

		const std::filesystem::path path = fileDialogResult;
		if(!std::filesystem::is_directory(path) || !std::filesystem::is_empty(path)) {
			Logger::error("Path '%s' is not an empty directory!", path.string().c_str());
			return false;
		}

		// TODO: is this a memory leak?
		std::string name = tinyfd_inputBox("New project name", "Type name for your new project", "New Project");
		Project::create(name, path);
		on_project_open();
		return true;
	}

	bool EditorModule::open_project_dialog() {
		static constexpr std::array<const char *, 1> filters = {"*.fifproj"};
		const char *fileDialogResult = tinyfd_openFileDialog("Select fif project", NULL, filters.size(), filters.data(), "Fif project", false);

		if(!fileDialogResult)
			return false;

		const std::filesystem::path path = std::filesystem::path(fileDialogResult).parent_path();
		return open_project(path);
	}

	bool EditorModule::open_project(const std::filesystem::path &path) {
		if(Project::load(path)) {
			on_project_open();

			if(!Project::get_config().startingScene.empty())
				open_scene(Project::get_config().startingScene);

			return true;
		}

		return false;
	}

	void EditorModule::on_project_open() {
		m_ProjectLoaded = true;

		mp_Application->get_window().set_title(Project::get_config().name + " | Fifed"
#ifdef FIF_DEBUG
											   + " [DEBUG]"
#endif
		);

		ImGuiModule::get_instance()->delete_render_func(&EditorModule::render_project_manager_imgui);

		m_IconManager.add_icon(IconType::GITHUB, {{0.0f, 0.0f}, {230.0f, 225.0f}});
		m_IconManager.add_icon(IconType::LOGO, {{0.0f, 225.0f}, {48.0f, 48.0f}});
		m_IconManager.add_icon(IconType::PAUSE, {{48.0f, 225.0f}, {32.0f, 32.0f}});
		m_IconManager.add_icon(IconType::UNPAUSE, {{80.0f, 225.0f}, {32.0f, 32.0f}});
		m_IconManager.add_icon(IconType::STOP, {{80.0f, 225.0f}, {32.0f, 32.0f}});

		m_Shortcuts.emplace_back(GLFW_KEY_O, GLFW_MOD_CONTROL, "Open a scene", std::bind(&EditorModule::open_scene_dialog, this));
		m_Shortcuts.emplace_back(GLFW_KEY_S, GLFW_MOD_CONTROL, "Save current scene", std::bind(&EditorModule::save_scene, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F, 0, "Follow/focus selected entity", std::bind(&EditorModule::follow_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_DELETE, 0, "Delete selected entity", std::bind(&EditorModule::delete_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F5, 0, "Start/Stop the game", std::bind(&EditorModule::toggle_runtime, this));

		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		mp_ViewportPanel = add_panel<ViewportPanel>(m_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>(mp_Application->get_scene());
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>(m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>(*mp_InspectorPanel);

		ImGuiModule::get_instance()->add_render_func(&EditorModule::render_editor_imgui);
	}

	void EditorModule::save_scene() {
		// TODO: Confirmation prompt when in play mode

		if(m_CurrentScenePath.empty()) {
			const char *filter = "*.yaml";
			const char *result = tinyfd_saveFileDialog("Save scene", (Project::get_absolute_path() + "scene.yaml").c_str(), 1, &filter, "YAML file");

			if(result == NULL) {
				Logger::error("Failed to get path of the scene to save");
				return;
			}

			m_CurrentScenePath = result;
		}

		SceneSerializer serializer(mp_Application->get_scene());
		serializer.serialize(m_CurrentScenePath);
		Logger::info("Scene saved to: %s", m_CurrentScenePath.c_str());
	}

	void EditorModule::open_scene_dialog() {
		const char *filter = "*.yaml";
		const char *path = tinyfd_openFileDialog("Select scene", Project::get_absolute_path().c_str(), 1, &filter, "YAML scene", false);

		if(!path)
			return;

		open_scene(path);
	}

	void EditorModule::open_scene(const std::filesystem::path &path) {
		if(path.empty()) {
			Logger::error("Cannot open scene, invalid path!");
			return;
		}

		if(Project::get_config().startingScene.empty()) {
			Project::get_config().startingScene = path;
			Project::save();
		}

		m_CurrentScenePath = path;

		SceneSerializer serializer(mp_Application->get_scene());
		serializer.deserialize(path);

		Logger::info("Scene loaded: %s", path.c_str());
	}

	void EditorModule::set_play_mode(const bool playMode) {
		m_PlayMode = playMode;

		// Load the scene if play mode has ended
		if(!playMode) {
			if(!m_CurrentScenePath.empty())
				open_scene(m_CurrentScenePath);

		} else
			save_scene();// Save the scene if play mode has been entered

		mp_Application->set_pause(!playMode);
	}

	void EditorModule::follow_selected_entity() {
		if(TransformComponent *trans = mp_InspectorPanel->m_SelectedEntity.try_get_component<TransformComponent>())
			GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = trans->position;
		else
			GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = vec2(0, 0);
	}

	void EditorModule::delete_selected_entity() {
		if(mp_InspectorPanel->m_SelectedEntity)
			mp_InspectorPanel->m_SelectedEntity.delete_self();
	}

	void EditorModule::toggle_runtime() {
		set_play_mode(!m_PlayMode);
	}

	void EditorModule::on_event(Event &event) {
		if(mp_ViewportPanel) {
			m_CameraController.on_event(event, mp_ViewportPanel->is_hovered());
		}

		EventDispatcher::dispatch<KeyPressedEvent>(event, [&](KeyPressedEvent &keyEvent) {
			for(const Shortcut &shortcut : m_Shortcuts) {
				if(keyEvent.get_key_code() == shortcut.get_key() && (shortcut.get_modifier_bits() == 0 || (keyEvent.get_modifier_bits() & shortcut.get_modifier_bits()))) {
					shortcut.callback();
					return true;
				}
			}

			return false;
		});
	}
}// namespace fifed
