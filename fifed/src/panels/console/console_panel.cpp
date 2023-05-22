#include "console_panel.hpp"

namespace fifed {
#define RGBU8_TO_RGBAF32(r, g, b) ImVec4((r) / 255.0f, (g) / 255.0f, (b) / 255.0f, 1.0f)
	static ConsolePanel *s_Instance;

	static constexpr LogTypeData LOG_TYPE_INFO_DATA{.color = RGBU8_TO_RGBAF32(199, 199, 199), .prefix = "INFO"};
	static constexpr LogTypeData LOG_TYPE_WARN_DATA{.color = RGBU8_TO_RGBAF32(255, 197, 145), .prefix = "WARN"};
	static constexpr LogTypeData LOG_TYPE_ERROR_DATA{.color = RGBU8_TO_RGBAF32(255, 102, 102), .prefix = "ERROR"};
	static constexpr LogTypeData LOG_TYPE_DEBUG_DATA{.color = RGBU8_TO_RGBAF32(97, 149, 127), .prefix = "DEBUG"};
	static constexpr std::array<const LogTypeData *, 4> LOG_TYPE_DATA_LOOKUP = {{&LOG_TYPE_INFO_DATA, &LOG_TYPE_WARN_DATA, &LOG_TYPE_ERROR_DATA, &LOG_TYPE_DEBUG_DATA}};

	ConsolePanelEntry::ConsolePanelEntry(const LogTypeData *data, const char *msg) : logTypeData(data), message(msg) {
		const time_t now = time(nullptr);
		const tm *timeInfo = localtime(&now);
		strftime(timePrefixBuffer.data(), timePrefixBuffer.size() - 1, "%H:%M:%S", timeInfo);
	}

	ConsolePanel::ConsolePanel() {
		s_Instance = this;
		Logger::add_callback(&ConsolePanel::logger_callback);
	}

	ConsolePanel::~ConsolePanel() {
		Logger::delete_callback(&ConsolePanel::logger_callback);
	}

	void ConsolePanel::logger_callback(Logger::LogType logType, const char *msg) {
		if(!s_Instance)
			return;

		if(s_Instance->m_Output.size() >= MAX_LINES)
			s_Instance->m_Output.pop_front();

		s_Instance->m_Output.emplace_back(LOG_TYPE_DATA_LOOKUP[static_cast<int>(logType)], msg);
	}

	void ConsolePanel::on_render() {
		if(ImGui::Button("Clear"))
			m_Output.clear();

		if(ImGui::BeginListBox("###ConsolePanelList", ImVec2(-FLT_MIN, -FLT_MIN))) {
			for(const ConsolePanelEntry &entry : m_Output) {
				ImGui::PushTextWrapPos();
				ImGui::TextColored(entry.logTypeData->color, "(%s) [%s] %s", entry.timePrefixBuffer.data(), entry.logTypeData->prefix, entry.message.c_str());
				ImGui::PopTextWrapPos();
			}

			// If the list is scrolled all the way down
			if(ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);

			ImGui::EndListBox();
		}
	}
}// namespace fifed
