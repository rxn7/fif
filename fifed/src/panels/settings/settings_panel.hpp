#pragma once

#include "../editor_panel.hpp"
#include "grid.hpp"

#include "fif/gfx/frame_buffer.hpp"

namespace fifed {
	class SettingsPanel final : public EditorPanel {
	public:
		SettingsPanel(Grid &grid, FrameBuffer &frameBuffer);
		PANEL_NAME("Settings")
		void on_render() override;

	private:
		Grid &m_Grid;
		FrameBuffer &m_FrameBuffer;
	};
}// namespace fifed
