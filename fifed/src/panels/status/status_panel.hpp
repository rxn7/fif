#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class StatusPanel : public EditorPanel {
	public:
		PANEL_NAME("Status")
		void on_render() override;
	};
};// namespace fifed
