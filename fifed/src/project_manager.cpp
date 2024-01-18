#include "project_manager.hpp"
#include "fifed_module.hpp"
#include "project.hpp"

#include <fstream>
#include <sstream>
#include <tinyfiledialogs.h>

namespace fifed {
	static const std::filesystem::path s_ProjectListPath = "./recent_projects.txt";

	ProjectManager::ProjectManager(FifedModule &fifedModule) : Stage(fifedModule) {
		Project::s_ActiveProject.reset();
		load_project_list();
	}

	void ProjectManager::update() {
		if(m_ProjectLoaded) {
			std::filesystem::path projectRootDir = Project::get_root_dir();

			const bool projectAlreadyOnList =
				std::find_if(m_ProjectListItems.begin(), m_ProjectListItems.end(), [&projectRootDir](ProjectListItem &item) -> bool { return item.path == projectRootDir; }) != m_ProjectListItems.end();

			if(!projectAlreadyOnList) {
				m_ProjectListItems.push_back({Project::get_config().name, projectRootDir});
				save_project_list();
			}

			m_FifedModule.on_project_open();
		}
	}

	void ProjectManager::render_imgui() {
		ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		bool openDeleteModal = false;
		static i32 deleteModalContextItemIdx = -1;

		if(ImGui::Begin("Project Manager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize)) {
			constexpr ImVec2 BUTTON_SIZE{180, 40};

			if(ImGui::Button("Create new project", BUTTON_SIZE))
				m_ProjectLoaded = create_project_dialog();

			ImGui::SameLine();

			if(ImGui::Button("Open project", BUTTON_SIZE))
				m_ProjectLoaded = open_project_dialog();

			ImGui::Dummy(vec2(0.0f, 20.0f));

			if(m_ProjectListItems.size() == 0) {
				ImGui::Text("No recent projects.");
			} else {
				ImGui::Text("Recent Projects:");

				if(ImGui::BeginTable("Project List", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingFixedFit)) {
					ImGui::TableSetupColumn("Name");
					ImGui::TableSetupColumn("Path");
					ImGui::TableSetupColumn("Last modified");
					ImGui::TableHeadersRow();

					for(u32 i = 0; i < m_ProjectListItems.size(); ++i) {
						const ProjectListItem &item = m_ProjectListItems[i];

						ImGui::TableNextColumn();
						if(ImGui::Selectable(std::format("{}##{}", item.name, i).c_str(), false, ImGuiSelectableFlags_SpanAllColumns)) {
							m_ProjectLoaded = open_project(item.path);
						}
						if(ImGui::BeginPopupContextItem(std::format("ProjectListEntryContext##{}", i).c_str())) {
							if(ImGui::SmallButton("Delete")) {
								deleteModalContextItemIdx = i;
								openDeleteModal = true;
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}

						ImGui::TableNextColumn();
						ImGui::Text("%s", item.path.string().c_str());

						ImGui::TableNextColumn();
						ImGui::Text("%s", item.lastTimeModifiedString.c_str());
					}
					ImGui::EndTable();
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		static const char *projectItemListDeleteModalName = "Are you sure?###ProjectItemListDelete";

		if(openDeleteModal) {
			ImGui::OpenPopup(projectItemListDeleteModalName);
			openDeleteModal = false;
		}

		const ImVec2 popupModalSize = {400, 200};
		ImGui::SetNextWindowSize(popupModalSize, ImGuiCond_Always);
		ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, {0.5f, 0.5f});

		if(ImGui::BeginPopupModal(projectItemListDeleteModalName, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ProjectListItem &item = m_ProjectListItems[deleteModalContextItemIdx];
			ImGui::TextWrapped("Delete project \"%s\"?", item.name.c_str());

			if(ImGui::Button("Yes")) {
				m_ProjectListItems.erase(m_ProjectListItems.begin() + deleteModalContextItemIdx);
				save_project_list();
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if(ImGui::Button("No")) {
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void ProjectManager::load_project_list() {
		// TODO: Use yaml instead of txt
		std::ifstream fileStream(s_ProjectListPath);

		if(!fileStream.is_open()) {
			Logger::warn("Failed to open recent_projects.txt, you can ignore this warning if running fifed for the first time.");
			return;
		}

		std::string line;
		while(std::getline(fileStream, line)) {
			add_project_list_item(line);
		}
	}

	void ProjectManager::add_project_list_item(const std::filesystem::path &path) {
		if(!std::filesystem::is_directory(path)) {
			Logger::error("Failed to add project to project list: '%s' is not a directory!", path.string().c_str());
			return;
		}

		if(!std::filesystem::exists(path)) {
			Logger::error("Failed to add project to project list: '%s' does not exist!", path.string().c_str());
			return;
		}

		std::filesystem::path projectFilePath = path / Project::project_filename();

		if(!std::filesystem::exists(projectFilePath)) {
			Logger::error("Failed to add project to project list: '%s' directory does not contain 'project.fifproj' file!", path.string().c_str());
			return;
		}

		// TODO: Read only Config part of .fifproj?

		ProjectListItem projectListItem;
		projectListItem.path = path;

		std::ifstream fileStream(projectFilePath);
		std::stringstream ss;
		ss << fileStream.rdbuf();
		const YAML::Node yaml = YAML::Load(ss);
		const YAML::Node configNode = yaml["Config"];
		projectListItem.name = configNode["Name"].as<std::string>();

		projectListItem.lastTimeModifiedString = std::format("{:%Y/%m/%d %H:%M}", std::filesystem::last_write_time(path));

		m_ProjectListItems.push_back(projectListItem);
	}

	void ProjectManager::save_project_list() {
		std::ofstream fileStream(s_ProjectListPath);

		if(!fileStream.is_open()) {
			Logger::error("Failed to open recent_projects.txt for writing");
			return;
		}

		for(const ProjectListItem &item : m_ProjectListItems) {
			fileStream << item.path.string() << "\n";
		}

		fileStream.close();
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

	bool ProjectManager::open_project(const std::filesystem::path &path) { return Project::load(path); }
}// namespace fifed
