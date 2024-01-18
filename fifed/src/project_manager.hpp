#pragma once

#include "stage.hpp"

#include <fif/core/project.hpp>

namespace fifed {
	class FifedModule;

	struct ProjectListItem final {
		std::string name;
		std::filesystem::path path;
		std::string lastTimeModifiedString = "?";
	};

	class ProjectManager final : public Stage {
	public:
		ProjectManager(FifedModule &editorModule);

		void update() override;
		void render_imgui() override;

	private:
		void load_project_list();
		void save_project_list();
		void add_project_list_item(const std::filesystem::path &path);
		void save_project_to_recent_projects();
		bool create_project_dialog();
		bool open_project_dialog();
		bool open_project(const std::filesystem::path &path);

	private:
		bool m_ProjectLoaded = false;
		std::vector<ProjectListItem> m_ProjectListItems;
	};
}// namespace fifed
