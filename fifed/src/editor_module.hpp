#pragma once

#include "camera_controller.hpp"
#include "entt/entity/group.hpp"
#include "grid.hpp"
#include "icon_manager.hpp"
#include "panels/editor_panel.hpp"
#include "panels/inspector/inspector_panel.hpp"
#include "panels/viewport/viewport_panel.hpp"

#include "fif/gfx/frame_buffer.hpp"
#include "fif/gfx/texture.hpp"
#include "shortcut.hpp"

namespace fifed {
	class EditorModule final : public Module {
	public:
		FIF_MODULE(EditorModule)

		EditorModule();
		virtual ~EditorModule();

		inline FrameBuffer &get_frame_buffer() { return m_FrameBuffer; }

	protected:
		void on_start() override;
		void on_render() override;
		void pre_render() override;
		void on_update() override;
		void on_event(Event &event) override;
		void load_default_layout();

		template<class T, class... Args> T *add_panel(Args &&...args) {
			std::unique_ptr<EditorPanel> &panel = m_Panels.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			panel->mp_IconManager = &m_IconManager;
			return reinterpret_cast<T *>(panel.get());
		}

	private:
		void save_scene();
		void open_scene();
		void follow_selected_entity();
		void delete_selected_entity();
		static void on_render_im_gui();

	private:
		ViewportPanel *mp_ViewportPanel;
		InspectorPanel *mp_InspectorPanel;

		bool m_AboutWindowOpen = false;
		bool m_ShortcutsWindowOpen = false;
		std::vector<std::unique_ptr<EditorPanel>> m_Panels;
		std::vector<Shortcut> m_Shortcuts;

		IconManager m_IconManager;
		FrameBuffer m_FrameBuffer;
		Grid m_Grid;
		CameraController m_CameraController;
	};
}// namespace fifed
