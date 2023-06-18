#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class ViewportPanel final : public EditorPanel {
	public:
		PANEL_NAME("Viewport")

		ViewportPanel(FrameBuffer &frameBuffer);

		void on_render() override;
		inline bool is_hovered() const { return m_Hovered; }

	private:
		std::unique_ptr<Texture> mp_PauseTexture, mp_UnpauseTexture;
		FrameBuffer &m_FrameBuffer;
		bool m_Hovered = false;
	};
}// namespace fifed
