#include "resource_browser.hpp"

#include <fif/core/project.hpp>

namespace fifed {
	ResourceBrowserPanel::ResourceBrowserPanel(Editor &editor) : EditorPanel(editor), m_RootDirectory(Project::get_root_dir()), m_CurrentDirectory(m_RootDirectory) {}

	void ResourceBrowserPanel::on_render() {
		const bool isInRoot = m_CurrentDirectory == m_RootDirectory;

		if(!isInRoot) {
			const std::string relativePath = std::filesystem::relative(m_CurrentDirectory, m_RootDirectory).string();
			if(ImGui::ArrowButton(relativePath.c_str(), ImGuiDir_Left)) {
				current_directory_go_back();
			}
		}

		// TODO: Use clipper to iterate over only visible entries
		// TODO: Call filesystem api on interval or when something changes in the directory.

		for(const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
			const std::string entryFileName = entry.path().filename().string();
			const std::string buttonId = entryFileName + "##resourceBrowserEntry";

			if(entry.is_directory()) {
				if(ImGui::Button(buttonId.c_str())) {
					m_CurrentDirectory = entry.path();
				}
			} else {
				if(ImGui::Button(buttonId.c_str())) {
				}
			}
		}
	}

	void ResourceBrowserPanel::current_directory_go_back() { m_CurrentDirectory = m_CurrentDirectory.parent_path(); }
}// namespace fifed
