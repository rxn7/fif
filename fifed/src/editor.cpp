#include "editor.hpp"
#include "fifed_module.hpp"
#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/resource_browser/resource_browser.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "project_manager.hpp"

#include <fif/core/ecs/components/transform_component.hpp>
#include <fif/core/event/key_event.hpp>
#include <fif/core/project.hpp>
#include <fif/core/serialization/scene_serializer.hpp>
#include <fif/core/system.hpp>

#include <fstream>
#include <tinyfiledialogs.h>

namespace fifed {
	Editor::Editor(FifedModule &fifedModule) : Stage(fifedModule), m_FrameBuffer({0, 0}), m_Grid(fif::gfx::GfxModule::get_instance()->get_renderer2D().get_camera(), m_FrameBuffer) {
		m_FifedModule.get_application()->get_window().set_title(Project::get_config().name + " | Fifed"
#ifdef FIF_DEBUG
																+ " [DEBUG]");
#endif

		init_shortcuts();
		init_panels();

		const std::filesystem::path startingScenePath = Project::get_config().startingScenePath;
		if(!startingScenePath.empty())
			open_scene(startingScenePath);
	}

	void Editor::init_shortcuts() {
		m_Shortcuts.emplace_back(GLFW_KEY_O, GLFW_MOD_CONTROL, "Open a scene", std::bind(&Editor::open_scene_dialog, this));
		m_Shortcuts.emplace_back(GLFW_KEY_S, GLFW_MOD_CONTROL, "Save current scene", std::bind(&Editor::save_scene, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F, 0, "Follow/focus selected entity", std::bind(&Editor::follow_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_DELETE, 0, "Delete selected entity", std::bind(&Editor::delete_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F5, 0, "Toggle play mode", std::bind(&Editor::toggle_play_mode, this));
	}

	void Editor::init_panels() {
		mp_ViewportPanel = add_panel<ViewportPanel>(*this, m_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>(*this, m_FifedModule.get_application()->get_scene());
		add_panel<PerformancePanel>(*this);
		add_panel<SettingsPanel>(*this, m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>(*this, *mp_InspectorPanel);
		add_panel<ResourceBrowserPanel>(*this);
		add_panel<ConsolePanel>(*this);
	}

	void Editor::update() {
		m_CameraController.update();
	}

	void Editor::pre_render() {
		m_FrameBuffer.start();
		m_Grid.render();
	}

	void Editor::render() {
		m_FrameBuffer.end();
	}

	void Editor::save_scene() {
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

		SceneSerializer serializer(m_FifedModule.get_application()->get_scene());
		serializer.serialize(m_CurrentScenePath);
		Logger::info("Scene saved to: %s", m_CurrentScenePath.c_str());

		std::filesystem::path &startingScenePath = Project::get_config().startingScenePath;

		if(startingScenePath.empty() || !std::filesystem::exists(Project::get_resource_path(startingScenePath))) {
			startingScenePath = std::filesystem::relative(m_CurrentScenePath, Project::get_root_dir());
			Project::save();
		}
	}

	void Editor::open_scene(const std::filesystem::path &path) {
		if(path.empty()) {
			Logger::error("Cannot open scene, invalid path!");
			return;
		}

		m_CurrentScenePath = path;

		SceneSerializer serializer(m_FifedModule.get_application()->get_scene());
		serializer.deserialize(path);

		Logger::info("Scene loaded: %s", path.c_str());
	}

	void Editor::open_scene_dialog() {
		const char *filter = "*.yaml";
		const char *path = tinyfd_openFileDialog("Select scene", Project::get_absolute_path().c_str(), 1, &filter, "YAML scene", false);

		if(!path)
			return;

		open_scene(path);
	}

	void Editor::set_play_mode(const bool playMode) {
		m_PlayMode = playMode;

		// Load the scene if play mode has ended
		if(!playMode) {
			if(!m_CurrentScenePath.empty())
				open_scene(m_CurrentScenePath);

		} else
			save_scene();// Save the scene if play mode has been entered

		m_FifedModule.get_application()->set_pause(!playMode);
	}

	void Editor::follow_selected_entity() {
		if(TransformComponent *trans = mp_InspectorPanel->m_SelectedEntity.try_get_component<TransformComponent>())
			GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = trans->position;
		else
			GfxModule::get_instance()->get_renderer2D().get_camera().m_Position = vec2(0, 0);
	}

	void Editor::delete_selected_entity() {
		if(mp_InspectorPanel->m_SelectedEntity)
			mp_InspectorPanel->m_SelectedEntity.delete_self();
	}

	void Editor::on_event(Event &event) {
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

	void Editor::render_imgui() {
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::Button("About"))
				m_AboutWindowOpen = true;

			if(ImGui::BeginMenu("Scene")) {
				if(ImGui::MenuItem("Save", "Ctrl+S"))
					save_scene();

				if(ImGui::MenuItem("Load", "Ctrl+O"))
					open_scene_dialog();

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Project")) {
				if(ImGui::MenuItem("Save"))
					Project::save();

				if(ImGui::MenuItem("Open Project Manager"))
					m_FifedModule.mp_Stage = std::make_unique<ProjectManager>(m_FifedModule);

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Layout")) {
				if(ImGui::MenuItem("Load Default"))
					m_FifedModule.load_default_layout();

				if(ImGui::MenuItem("Save"))
					ImGui::SaveIniSettingsToDisk("layout.ini");

				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Shortcuts"))
				m_ShortcutsWindowOpen = true;

			ImGui::EndMainMenuBar();
		}

		if(m_AboutWindowOpen) {
			ImGui::SetNextWindowSize(ImVec2(350, 200));
			if(ImGui::Begin("About", &m_AboutWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) {
				ImGui::Text("Fif v%u.%u.%u", 0, 0, 0);// TODO: version(major,minor,patch)
				if(ImGui::CollapsingHeader("License")) {
					static std::ifstream fileStream("LICENSE", std::ios::in | std::ios::binary);
					static std::string content(std::istreambuf_iterator<char>(fileStream), {});
					ImGui::TextWrapped("%s", content.c_str());
				}

				ImGui::Text("Author: ");
				ImGui::SameLine();
				if(ImGui::Button("Maciej Niziołek###authorBtn"))
					System::open_url("https://maciejniziolek.xyz");

				ImGui::Text("Source: ");
				ImGui::SameLine();
				if(FifedModule::get_instance()->get_icon_manager().imgui_button("sourceBtn", IconType::GITHUB))
					System::open_url("https://github.com/rxn7/fif");
			}
			ImGui::End();
		}

		if(m_ShortcutsWindowOpen) {
			ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);

			// TODO: Use clipper to render only visible entries
			if(ImGui::Begin("Shortcuts", &m_ShortcutsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
				for(const Shortcut &shortcut : m_Shortcuts)
					ImGui::TextWrapped("%s", shortcut.get_description().c_str());

			ImGui::End();
		}

		if(ImGuiModule::get_instance()->begin_dockspace()) {
			for(std::shared_ptr<EditorPanel> &panel : m_Panels)
				panel->render();
		}

		ImGui::End();
	}
}// namespace fifed
