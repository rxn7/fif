#pragma once

#include "../editor_panel.hpp"

#include <fif/core/invokable.hpp>

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

	class ConsolePanel final : public EditorPanel {
	public:
		PANEL_NAME("Console")
		ConsolePanel(Editor &editor);
		~ConsolePanel();
		void on_render() override;

	private:
		void logger_callback(Logger::LogType logType, const char *msg);

	private:
		Callback<Logger::LogType, const char *> m_LoggerCallback;
		std::deque<ConsolePanelEntry> m_Output;
	};
}// namespace fifed
