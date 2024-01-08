#pragma once

#include "camera_controller.hpp"
#include "grid.hpp"
#include "panels/editor_panel.hpp"
#include "panels/inspector/inspector_panel.hpp"
#include "panels/viewport/viewport_panel.hpp"
#include "shortcut.hpp"
#include "stage.hpp"

#include <fif/core/invokable.hpp>
#include <fif/gfx/frame_buffer.hpp>

namespace fifed {
	class Editor final : public Stage {
	public:
		Editor(FifedModule &fifedModule);

		void pre_render() override;
		void render() override;
		void update() override;
		void render_imgui() override;
		void on_event(Event &event) override;
		void set_play_mode(const bool playMode);

		inline bool is_play_mode() const { return m_PlayMode; }

		inline void toggle_play_mode() { set_play_mode(!m_PlayMode); }

	private:
		void save_project();
		void open_scene_dialog();
		void open_scene(const std::filesystem::path &path);
		void follow_selected_entity();
		void delete_selected_entity();

		void init_shortcuts();
		void init_panels();

		inline FrameBuffer &get_frame_buffer() { return m_FrameBuffer; }

		template<class T, class... Args> std::shared_ptr<T> add_panel(Args &&...args) {
			std::shared_ptr<EditorPanel> &panel = m_Panels.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
			return std::static_pointer_cast<T, EditorPanel>(panel);
		}

	private:
		std::filesystem::path m_CurrentScenePath = "";

		FrameBuffer m_FrameBuffer;
		Grid m_Grid;
		CameraController m_CameraController;

		std::vector<std::shared_ptr<EditorPanel>> m_Panels;
		std::vector<Shortcut> m_Shortcuts;

		std::shared_ptr<ViewportPanel> mp_ViewportPanel = nullptr;
		std::shared_ptr<InspectorPanel> mp_InspectorPanel = nullptr;

		bool m_OpenProjectManager = false;
		bool m_PlayMode = false;
		bool m_AboutWindowOpen = false;
		bool m_ShortcutsWindowOpen = false;
	};
}// namespace fifed
