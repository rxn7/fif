#pragma once

#include "../editor_panel.hpp"

class ViewportPanel final : public EditorPanel {
public:
	ViewportPanel(FrameBuffer &frameBuffer);
	const char *get_title() const override;
	void on_render() override;
	inline bool is_hovered() const { return m_Hovered; }

private:
	FrameBuffer &m_FrameBuffer;
	bool m_Hovered = false;
};
