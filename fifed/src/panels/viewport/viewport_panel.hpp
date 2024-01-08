#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class ViewportPanel final : public EditorPanel {
	public:
		ViewportPanel(Editor &editor, FrameBuffer &frameBuffer);
		PANEL_NAME("Viewport")

		void on_render() override;
		inline bool is_hovered() const {
			return m_Hovered;
		}

	public:
		bool m_Resize = false;

	private:
		FrameBuffer &m_FrameBuffer;
		vec2 m_LastSize{0, 0};
		bool m_Hovered = false;
	};
}// namespace fifed
