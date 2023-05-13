#include "console_panel.hpp"
#include "imgui.h"

namespace fifed {
	ConsolePanel::ConsolePanel() {
		m_Output.reserve(MAX_LINES);
		Logger::add_callback([&output = m_Output]([[maybe_unused]] Logger::LogType type, const char *msg) {
			if(output.size() >= MAX_LINES)
				output.erase(output.begin());

			ConsoleEntry entry;

			switch(type) {
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

			output.push_back(entry);
		});
	}

	void ConsolePanel::on_render() {
		if(ImGui::Button("Clear"))
			m_Output.clear();

		if(ImGui::BeginListBox("###ConsolePanelList", ImVec2(-FLT_MIN, -FLT_MIN))) {
			for(const ConsoleEntry &entry : m_Output) {
				ImGui::TextColored(entry.color, "%s", entry.message.c_str());
			}

			// If the list is scrolled all the way down
			if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndListBox();
		}
	}
}// namespace fifed
