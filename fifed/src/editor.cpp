#include "editor.hpp"
#include "fifed_module.hpp"

#include "panels/console/console_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "serialization/scene_serializer.hpp"

#include <fif/core/ecs/components/transform_component.hpp>
#include <fif/core/event/key_event.hpp>
#include <fif/core/project.hpp>

#include <fstream>
#include <tinyfiledialogs.h>

namespace fifed {
	Editor::Editor(FifedModule &fifedModule) :
		Stage(fifedModule), m_IconManager("assets/textures/icons.png"), m_FrameBuffer({0, 0}), m_Grid(fif::gfx::GfxModule::get_instance()->get_renderer2D().get_camera(), m_FrameBuffer) {
		m_FifedModule.get_application()->get_window().set_title(Project::get_config().name + " | Fifed"
#ifdef FIF_DEBUG
																+ " [DEBUG]");
#endif

		m_IconManager.add_icon(IconType::GITHUB, {{0.0f, 0.0f}, {230.0f, 225.0f}});
		m_IconManager.add_icon(IconType::LOGO, {{0.0f, 225.0f}, {48.0f, 48.0f}});
		m_IconManager.add_icon(IconType::PAUSE, {{48.0f, 225.0f}, {32.0f, 32.0f}});
		m_IconManager.add_icon(IconType::UNPAUSE, {{80.0f, 225.0f}, {32.0f, 32.0f}});
		m_IconManager.add_icon(IconType::STOP, {{80.0f, 225.0f}, {32.0f, 32.0f}});

		m_Shortcuts.emplace_back(GLFW_KEY_O, GLFW_MOD_CONTROL, "Open a scene", std::bind(&Editor::open_scene_dialog, this));
		m_Shortcuts.emplace_back(GLFW_KEY_S, GLFW_MOD_CONTROL, "Save current scene", std::bind(&Editor::save_scene, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F, 0, "Follow/focus selected entity", std::bind(&Editor::follow_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_DELETE, 0, "Delete selected entity", std::bind(&Editor::delete_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F5, 0, "Toggle play mode", std::bind(&Editor::toggle_play_mode, this));

		mp_ViewportPanel = add_panel<ViewportPanel>(*this, m_FrameBuffer);
		mp_InspectorPanel = add_panel<InspectorPanel>(*this, m_FifedModule.get_application()->get_scene());
		add_panel<PerformancePanel>(*this);
		add_panel<SettingsPanel>(*this, m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>(*this, *mp_InspectorPanel);

		if(!Project::get_config().startingScene.empty())
			open_scene(Project::get_config().startingScene);
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
	}

	void Editor::open_scene(const std::filesystem::path &path) {
		if(path.empty()) {
			Logger::error("Cannot open scene, invalid path!");
			return;
		}

		if(Project::get_config().startingScene.empty()) {
			Project::get_config().startingScene = std::filesystem::relative(path, Project::get_root_dir());
			Project::save();
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
				if(m_IconManager.imgui_button("Source", IconType::GITHUB)) {
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

		if(m_ShortcutsWindowOpen) {
			ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);
			// TODO: scrollbar here
			if(ImGui::Begin("Shortcuts", &m_ShortcutsWindowOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking))
				for(const Shortcut &shortcut : m_Shortcuts)
					ImGui::TextWrapped("%s", shortcut.get_description().c_str());

			ImGui::End();
		}

		if(ImGuiModule::get_instance()->begin_dockspace())
			for(std::shared_ptr<EditorPanel> &panel : m_Panels)
				panel->render();

		ImGui::End();
	}
}// namespace fifed
