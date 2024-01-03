#include "project_manager.hpp"
#include "fifed_module.hpp"
#include "project.hpp"

#include <tinyfiledialogs.h>

namespace fifed {
	ProjectManager::ProjectManager(FifedModule &fifedModule) : Stage(fifedModule) {
	}

	void ProjectManager::update() {
		if(m_ProjectLoaded)
			m_FifedModule.on_project_open();
	}

	void ProjectManager::render_imgui() {
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		if(ImGui::Begin("Project Manager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize)) {
			constexpr ImVec2 BUTTON_SIZE{200, 100};

			ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - BUTTON_SIZE.x);
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.5f - BUTTON_SIZE.y);

			if(ImGui::Button("Create new project", BUTTON_SIZE))
				m_ProjectLoaded = create_project_dialog();

			ImGui::SameLine();

			if(ImGui::Button("Open project", BUTTON_SIZE))
				m_ProjectLoaded = open_project_dialog();

			ImGui::End();
		}

		ImGui::PopStyleVar();
	}

	bool ProjectManager::create_project_dialog() {
		const char *fileDialogResult = tinyfd_selectFolderDialog("Select directory", NULL);

		if(!fileDialogResult)
			return false;

		const std::filesystem::path path = fileDialogResult;
		if(!std::filesystem::is_directory(path) || !std::filesystem::is_empty(path)) {
			Logger::error("Path '%s' is not an empty directory!", path.string().c_str());
			return false;
		}

		std::string name = tinyfd_inputBox("New project name", "Type name for your new project", "New Project");
		Project::create(name, path);

		m_FifedModule.on_project_open();

		return true;
	}

	bool ProjectManager::open_project_dialog() {
		static constexpr std::array<const char *, 1> filters = {"*.fifproj"};
		const char *fileDialogResult = tinyfd_openFileDialog("Select fif project", NULL, filters.size(), filters.data(), "Fif project", false);

		if(!fileDialogResult)
			return false;

		const std::filesystem::path path = std::filesystem::path(fileDialogResult).parent_path();
		return open_project(path);
	}

	bool ProjectManager::open_project(const std::filesystem::path &path) {
		return Project::load(path);
	}
}// namespace fifed
