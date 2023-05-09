#include "console_panel.hpp"
#include "imgui.h"

namespace fifed {
	ConsolePanel::ConsolePanel() {
		m_Output.reserve(MAX_LINES);
		Logger::add_callback([&output = m_Output]([[maybe_unused]] Logger::LogType type, const char *msg) {
			if(output.size() > MAX_LINES)
				output.erase(output.begin());

			output.push_back(msg);
		});
	}

	void ConsolePanel::on_render() {
		if(ImGui::Button("Clear"))
			m_Output.clear();

		if(ImGui::BeginListBox("###ConsolePanelList", ImVec2(-FLT_MIN, -FLT_MIN))) {
			for(const std::string &str : m_Output)
				ImGui::Text("%s", str.c_str());

			// If the list is scrolled all the way down
			if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndListBox();
		}
	}
}// namespace fifed
