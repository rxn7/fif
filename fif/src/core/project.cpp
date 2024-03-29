#include "fif/core/project.hpp"
#include "fif/core/serialization/project_serializer.hpp"

namespace fif::core {
	Project::~Project() { Logger::debug("Project::dtor"); }

	void Project::create(const std::string &name, const std::filesystem::path &projectPath) {
		Application::get_instance().get_scene().clear();

		s_ActiveProject = std::make_shared<Project>();
		s_ActiveProject->m_RootDir = projectPath;
		s_ActiveProject->m_AbsolutePath = std::filesystem::absolute(Project::get_root_dir()).string() + "/";
		s_ActiveProject->m_Config.name = name;

		save();
	}

	bool Project::load(const std::filesystem::path &rootDirectory) {
		Application::get_instance().get_scene().clear();

		s_ActiveProject = std::make_shared<Project>();
		s_ActiveProject->m_RootDir = rootDirectory;
		s_ActiveProject->m_AbsolutePath = std::filesystem::absolute(Project::get_root_dir()).string() + "/";

		ProjectSerializer serializer(s_ActiveProject);
		return serializer.deserialize();
	}

	void Project::save() {
		ProjectSerializer serializer(s_ActiveProject);
		serializer.serialize();
	}
}// namespace fif::core
