#pragma once

#include "../editor_panel.hpp"
#include <deque>
#include <format>

namespace fifed {
	struct LogTypeData {
		const ImVec4 color;
		const char *prefix;
	};

	struct ConsolePanelEntry {
		ConsolePanelEntry(const LogTypeData *data, const char *msg);

		const LogTypeData *logTypeData;
		std::array<char, 10> timePrefixBuffer;
		const std::string message;
	};

	class ConsolePanel : public EditorPanel {
	public:
		PANEL_NAME("Console")
		ConsolePanel();
		~ConsolePanel();
		void on_render() override;

	private:
		static void logger_callback(Logger::LogType logType, const char *msg);

	private:
		static constexpr u32 MAX_LINES = 50;
		std::deque<ConsolePanelEntry> m_Output;
		u32 m_LineIdx;
	};
}// namespace fifed
