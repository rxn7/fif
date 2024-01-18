#include "editor.hpp"
#include "event/mouse_event.hpp"
#include "fifed_module.hpp"
#include "panels/console/console_panel.hpp"
#include "panels/inspector/inspector_panel.hpp"
#include "panels/performance/performance_panel.hpp"
#include "panels/resource_browser/resource_browser.hpp"
#include "panels/scene/scene_panel.hpp"
#include "panels/settings/settings_panel.hpp"
#include "panels/viewport/viewport_panel.hpp"
#include "project_manager.hpp"

#include <fif/core/ecs/components/transform_component.hpp>
#include <fif/core/event/key_event.hpp>
#include <fif/core/event/window_event.hpp>
#include <fif/core/project.hpp>
#include <fif/core/serialization/scene_serializer.hpp>
#include <fif/core/system.hpp>

#include <fstream>
#include <tinyfiledialogs.h>

namespace fifed {
	Editor::Editor(FifedModule &fifedModule) :
		Stage(fifedModule), m_SelectedEntity(Application::get_instance().get_scene(), entt::null), m_FrameBuffer({0, 0}), m_Grid(fif::gfx::GfxModule::get_instance().get_renderer2D().get_camera(), m_FrameBuffer), m_Gizmo(*this) {
		m_FifedModule.get_application()->get_window().set_title(Project::get_config().name + " | Fifed"
#ifdef FIF_DEBUG
																+ " [DEBUG]"
#endif
		);

		init_shortcuts();
		init_panels();

		const std::filesystem::path startingScenePath = Project::get_config().startingScenePath;
		if(!startingScenePath.empty())
			open_scene(startingScenePath);
	}

	void Editor::init_shortcuts() {
		m_Shortcuts.emplace_back(GLFW_KEY_O, GLFW_MOD_CONTROL, "Open a scene", std::bind(&Editor::open_scene_dialog, this));
		m_Shortcuts.emplace_back(GLFW_KEY_S, GLFW_MOD_CONTROL, "Save project", std::bind(&Editor::save_project, this));

		m_Shortcuts.emplace_back(GLFW_KEY_Q, 0, "Gizmo Mode: Translate", [this]() { m_Gizmo.m_Mode = GizmoMode::Translate; });
		m_Shortcuts.emplace_back(GLFW_KEY_W, 0, "Gizmo Mode: Scale", [this]() { m_Gizmo.m_Mode = GizmoMode::Scale; });

		m_Shortcuts.emplace_back(GLFW_KEY_F, 0, "Follow/focus selected entity", std::bind(&Editor::follow_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_D, GLFW_MOD_CONTROL, "Duplicate selected entity", [this]() {
			if(!m_SelectedEntity)
				return;

			m_SelectedEntity.m_ID = m_SelectedEntity.duplicate();
		});
		m_Shortcuts.emplace_back(GLFW_KEY_DELETE, 0, "Delete selected entity", std::bind(&Editor::delete_selected_entity, this));
		m_Shortcuts.emplace_back(GLFW_KEY_F5, 0, "Toggle play mode", std::bind(&Editor::toggle_play_mode, this));
	}

	void Editor::init_panels() {
		mp_ViewportPanel = add_panel<ViewportPanel>(m_FrameBuffer);
		add_panel<InspectorPanel>();
		add_panel<PerformancePanel>();
		add_panel<SettingsPanel>(m_Grid, m_FrameBuffer, m_CameraController);
		add_panel<ScenePanel>();
		add_panel<ResourceBrowserPanel>();
		add_panel<ConsolePanel>();
	}

	void Editor::update() {
		if(m_OpenProjectManager) {
			m_FifedModule.open_project_manager();
			return;
		}

		m_CameraController.update();
	}

	void Editor::pre_render() {
		m_FrameBuffer.start();
		m_Grid.render();
	}

	void Editor::render() { m_Gizmo.render(); }

	void Editor::post_render() { m_FrameBuffer.end(); }

	void Editor::end_frame() {}

	void Editor::save_project() {
		if(m_PlayMode) {
			if(!tinyfd_messageBox("Are you sure?", "Do you want to save? You are in play mode!", "yesno", "question", 0)) {
				return;
			}
		}

		if(m_CurrentScenePath.empty()) {
			const char *filter = "*.fifscene";
			const char *result = tinyfd_saveFileDialog("Save scene", (Project::get_absolute_path() + "scene.fifscene").c_str(), 1, &filter, "Fif Scene");

			if(result == NULL) {
				Logger::error("Failed to get path of the scene to save");
				return;
			}

			m_CurrentScenePath = result;
		}

		SceneSerializer serializer(m_FifedModule.get_application()->get_scene());
		if(!serializer.serialize(m_CurrentScenePath)) {
			Logger::error("Failed to save scene to: %s", m_CurrentScenePath.c_str());
		} else {
			Logger::debug("Scene saved to: %s", m_CurrentScenePath.c_str());
		}

		std::filesystem::path &startingScenePath = Project::get_config().startingScenePath;

		if(startingScenePath.empty() || !std::filesystem::exists(Project::get_resource_path(startingScenePath)))
			startingScenePath = std::filesystem::relative(m_CurrentScenePath, Project::get_root_dir());

		Project::save();
	}

	void Editor::open_scene(const std::filesystem::path &path) {
		if(path.empty()) {
			Logger::error("Cannot open scene, invalid path!");
			return;
		}

		m_CurrentScenePath = path;

		SceneSerializer serializer(m_FifedModule.get_application()->get_scene());
		if(!serializer.deserialize(path)) {
			Logger::error("Failed to load scene '%s'", path.c_str());
			return;
		}

		Logger::debug("Scene loaded: %s", path.c_str());
	}

	void Editor::open_scene_dialog() {
		const char *filter = "*.fifscene";
		const char *path = tinyfd_openFileDialog("Select scene", Project::get_absolute_path().c_str(), 1, &filter, "Fif Scene", false);

		if(!path)
			return;

		open_scene(path);
	}

	void Editor::set_play_mode(const bool playMode) {
		// Load the scene if play mode has ended
		if(!playMode) {
			if(!m_CurrentScenePath.empty()) {
				open_scene(m_CurrentScenePath);
			}
		} else {
			save_project();// Save the scene if play mode has been entered
		}

		m_FifedModule.get_application()->set_pause(!playMode);
		m_PlayMode = playMode;
	}

	void Editor::follow_selected_entity() {
		if(TransformComponent *trans = m_SelectedEntity.try_get_component<TransformComponent>())
			GfxModule::get_instance().get_renderer2D().get_camera().m_Position = trans->position;
		else
			GfxModule::get_instance().get_renderer2D().get_camera().m_Position = vec2(0, 0);
	}

	void Editor::delete_selected_entity() {
		if(m_SelectedEntity)
			m_SelectedEntity.delete_self();
	}

	void Editor::on_event(Event &event) {
		m_Gizmo.on_event(event);

		if(mp_ViewportPanel->is_hovered()) {
			m_CameraController.on_event(event);
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

		EventDispatcher::dispatch<MouseButtonPressedEvent>(event, [&](MouseButtonPressedEvent &mouseEvent) {
			if(mouseEvent.is_hanlded() || !mp_ViewportPanel->is_hovered() || mouseEvent.get_button() != GLFW_MOUSE_BUTTON_LEFT) {
				return false;
			}

			vec2 point = InputModule::get_instance().get_mouse_position() - GfxModule::get_instance().get_viewport_position();
			point.y = GfxModule::get_instance().get_viewport_size().y - point.y;

			const u32 hoveredEntity = get_frame_buffer().read_entity_id_buffer_pixel(point);
			if(hoveredEntity == Renderer2D::INVALID_ENTITY_ID) {
				m_SelectedEntity.m_ID = entt::null;
			} else {
				m_SelectedEntity.m_ID = (EntityID)hoveredEntity;
			}

			return true;
		});

		EventDispatcher::dispatch<WindowResizeEvent>(event, [&]([[maybe_unused]] WindowResizeEvent &ev) {
			mp_ViewportPanel->m_Resize = true;
			return false;
		});
	}

	void Editor::render_imgui() {
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::Button("About"))
				m_AboutWindowOpen = true;

			if(ImGui::BeginMenu("Scene")) {
				if(ImGui::MenuItem("Save", "Ctrl+S"))
					save_project();

				if(ImGui::MenuItem("Load", "Ctrl+O"))
					open_scene_dialog();

				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Project")) {
				if(ImGui::MenuItem("Save"))
					Project::save();

				if(ImGui::MenuItem("Open Project Manager"))
					m_OpenProjectManager = true;

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
				ImGui::Text("Fif v%u.%u.%u", fif::VERSION_MAJOR, fif::VERSION_MINOR, fif::VERSION_PATCH);
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
				if(m_FifedModule.get_icon_manager().imgui_button("sourceBtn", IconType::GITHUB))
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

		if(ImGuiModule::get_instance().begin_dockspace()) {
			for(std::shared_ptr<EditorPanel> &panel : m_Panels)
				panel->render();
		}

		ImGui::End();
	}
}// namespace fifed
