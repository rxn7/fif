#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class ConsolePanel : public EditorPanel {
	public:
		PANEL_NAME("Console")
		ConsolePanel();
		void on_render() override;

	private:
		struct ConsoleEntry {
			ImVec4 color;
			std::string message;
		};

		static constexpr u32 MAX_LINES = 50;
		std::vector<ConsoleEntry> m_Output;
	};
}// namespace fifed
