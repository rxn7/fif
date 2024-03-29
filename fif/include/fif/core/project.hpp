#pragma once

#include "fif/core/resource/resource_manager.hpp"

namespace fif::core {
	// This is serialized to project file
	struct ProjectConfig final {
		std::string name = "New Project";
		std::filesystem::path startingScenePath = "";
	};

	class Project final {
	public:
		~Project();

		static void create(const std::string &name, const std::filesystem::path &projectPath);
		static bool load(const std::filesystem::path &rootDirectory);
		static void save();

		inline static std::shared_ptr<Project> &get_active() { return s_ActiveProject; }

		static inline ProjectConfig &get_config() { return s_ActiveProject->m_Config; }

		static inline const std::filesystem::path &get_root_dir() { return s_ActiveProject->m_RootDir; }

		static inline std::filesystem::path get_resource_path(const std::filesystem::path &path) {
			if(!s_ActiveProject)
				return path;

			return s_ActiveProject->m_RootDir / path;
		}

		static inline const std::string &get_absolute_path() { return s_ActiveProject->m_AbsolutePath; }

		static constexpr std::string_view project_filename() { return "project.fifproj"; }

		static inline std::filesystem::path get_project_file_path() {
			std::filesystem::path projectFilePath = s_ActiveProject->m_RootDir;
			projectFilePath /= "project.fifproj";

			return projectFilePath;
		}

		static inline ResourceManager &get_resource_manager() { return s_ActiveProject->m_ResourceManager; }

	public:
		inline static std::shared_ptr<Project> s_ActiveProject;

	private:
		ProjectConfig m_Config;
		ResourceManager m_ResourceManager;
		std::filesystem::path m_RootDir;
		std::string m_AbsolutePath;

		friend class ProjectSerializer;
	};
}// namespace fif::core
