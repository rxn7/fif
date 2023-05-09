#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class ConsolePanel : public EditorPanel {
	public:
		PANEL_NAME("Console")
		ConsolePanel();
		void on_render() override;
		void logger_callback();

	private:
		static constexpr u32 MAX_LINES = 50;
		std::vector<std::string> m_Output;
	};
}// namespace fifed
