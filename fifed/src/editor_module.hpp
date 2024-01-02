#pragma once

#include "camera_controller.hpp"
#include "entt/entity/group.hpp"
#include "fif/gfx/frame_buffer.hpp"
#include "fif/gfx/texture.hpp"
#include "grid.hpp"
#include "icon_manager.hpp"
#include "panels/editor_panel.hpp"
#include "panels/inspector/inspector_panel.hpp"
#include "panels/viewport/viewport_panel.hpp"
#include "shortcut.hpp"

#include <memory>

namespace fifed {
	class EditorModule final : public Module {
	public:
		FIF_MODULE(EditorModule)

		EditorModule();
		virtual ~EditorModule();

		void set_play_mode(const bool playMode);

		inline bool is_runtime() const {
			return m_PlayMode;
		}

		inline FrameBuffer &get_frame_buffer() {
			return m_FrameBuffer;
		}

	protected:
		void on_start() override;
		void on_render() override;
		void pre_render() override;
		void on_update() override;
		void on_event(Event &event) override;
		void load_default_layout();

		template<class T, class... Args> std::shared_ptr<T> add_panel(Args &&...args) {
			std::shared_ptr<EditorPanel> &panel = m_Panels.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			panel->mp_IconManager = &m_IconManager;
			return std::static_pointer_cast<T, EditorPanel>(panel);
		}

	private:
		bool create_project_dialog();
		bool open_project_dialog();
		bool open_project(const std::filesystem::path &path);
		void on_project_open();

		void save_scene();
		void open_scene_dialog();
		void open_scene(const std::filesystem::path &path);

		void follow_selected_entity();
		void delete_selected_entity();

		void toggle_runtime();

		static void render_project_manager_imgui();
		static void render_editor_imgui();

	private:
		std::shared_ptr<ViewportPanel> mp_ViewportPanel = nullptr;
		std::shared_ptr<InspectorPanel> mp_InspectorPanel = nullptr;

		std::filesystem::path m_CurrentScenePath = "";
		bool m_AboutWindowOpen = false;
		bool m_ShortcutsWindowOpen = false;
		std::vector<std::shared_ptr<EditorPanel>> m_Panels;
		std::vector<Shortcut> m_Shortcuts;

		bool m_ProjectLoaded = false;
		bool m_PlayMode = false;

		IconManager m_IconManager;
		FrameBuffer m_FrameBuffer;
		Grid m_Grid;
		CameraController m_CameraController;
	};
}// namespace fifed
