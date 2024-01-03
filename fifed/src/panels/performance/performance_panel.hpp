#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class PerformancePanel final : public EditorPanel {
	public:
		PerformancePanel(Editor &editor);
		PANEL_NAME("Performance");

		void on_render() override;
	};
}// namespace fifed
