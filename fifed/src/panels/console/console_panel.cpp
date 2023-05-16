#include "console_panel.hpp"
#include "imgui.h"

namespace fifed {
	struct ConsoleEntry {
		ImVec4 color;
		std::string message;
	};

	static std::vector<ConsoleEntry> s_Output;

	ConsolePanel::ConsolePanel() {
		s_Output.clear();
		s_Output.reserve(MAX_LINES);
		Logger::add_callback(&ConsolePanel::logger_callback);
	}

	ConsolePanel::~ConsolePanel() {
		Logger::delete_callback(&ConsolePanel::logger_callback);
	}

	void ConsolePanel::logger_callback(Logger::LogType logType, const char *msg) {
		if(s_Output.size() >= MAX_LINES)
			s_Output.erase(s_Output.begin());

		ConsoleEntry entry;

		switch(logType) {
		default:
		case Logger::LogType::INFO:
			entry.color = ImGui::GetStyle().Colors[ImGuiCol_Text];
			entry.message = "[INFO] ";
			break;

		case Logger::LogType::WARN:
			entry.color = ImVec4(1.0f, 1.0f, 0.05f, 1.0f);
			entry.message = "[WARNING] ";
			break;

		case Logger::LogType::ERROR:
			entry.color = ImVec4(1.0f, 0.05f, 0.05f, 1.0f);
			entry.message = "[ERROR] ";
			break;

		case Logger::LogType::DEBUG:
			entry.color = ImVec4(0.05f, 0.9f, 0.05f, 1.0f);
			entry.message = "[DEBUG] ";
			break;
		}

		entry.message += msg;

		s_Output.push_back(entry);
	}

	void ConsolePanel::on_render() {
		if(ImGui::Button("Clear"))
			s_Output.clear();

		if(ImGui::BeginListBox("###ConsolePanelList", ImVec2(-FLT_MIN, -FLT_MIN))) {
			for(const ConsoleEntry &entry : s_Output)
				ImGui::TextColored(entry.color, "%s", entry.message.c_str());

			// If the list is scrolled all the way down
			if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndListBox();
		}
	}
}// namespace fifed
