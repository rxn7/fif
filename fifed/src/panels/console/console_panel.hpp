#pragma once

#include "../editor_panel.hpp"

namespace fifed {
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
	};
}// namespace fifed
