#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class ResourceBrowserPanel final : public EditorPanel {
	public:
		ResourceBrowserPanel(Editor &editor);
		PANEL_NAME("Resource Browser");

		void on_render() override;
		void current_directory_go_back();
		void on_file_double_click(const std::filesystem::path &path);

	private:
		std::filesystem::path m_RootDirectory;
		std::filesystem::path m_CurrentDirectory;
	};
}// namespace fifed
