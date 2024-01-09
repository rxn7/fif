#pragma once

#include "camera_controller.hpp"
#include "gizmo.hpp"
#include "grid.hpp"
#include "shortcut.hpp"
#include "stage.hpp"

#include <fif/core/invokable.hpp>
#include <fif/gfx/frame_buffer.hpp>

namespace fifed {
	class EditorPanel;
	class ViewportPanel;
	class InspectorPanel;

	class Editor final : public Stage {
	public:
		Editor(FifedModule &fifedModule);

		void pre_render() override;
		void update() override;
		void render() override;
		void post_render() override;
		void end_frame() override;
		void render_imgui() override;
		void on_event(Event &event) override;
		void set_play_mode(const bool playMode);

		inline bool is_play_mode() const { return m_PlayMode; }
		inline void toggle_play_mode() { set_play_mode(!m_PlayMode); }
		inline FrameBuffer &get_frame_buffer() { return m_FrameBuffer; }
		inline Gizmo &get_gizmo() { return m_Gizmo; }

	private:
		void save_project();
		void open_scene_dialog();
		void open_scene(const std::filesystem::path &path);
		void follow_selected_entity();
		void delete_selected_entity();

		void init_shortcuts();
		void init_panels();

		template<class T, class... Args> std::shared_ptr<T> add_panel(Args &&...args) {
			std::shared_ptr<EditorPanel> &panel = m_Panels.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			return std::static_pointer_cast<T, EditorPanel>(panel);
		}

	public:
		Entity m_SelectedEntity;

	private:
		std::filesystem::path m_CurrentScenePath = "";

		FrameBuffer m_FrameBuffer;
		Grid m_Grid;
		Gizmo m_Gizmo;
		CameraController m_CameraController;

		std::vector<Shortcut> m_Shortcuts;

		std::vector<std::shared_ptr<EditorPanel>> m_Panels;
		std::shared_ptr<ViewportPanel> mp_ViewportPanel = nullptr;

		bool m_OpenProjectManager = false;
		bool m_PlayMode = false;
		bool m_AboutWindowOpen = false;
		bool m_ShortcutsWindowOpen = false;
	};
}// namespace fifed
