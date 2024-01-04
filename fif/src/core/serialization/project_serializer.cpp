#include "fif/core/serialization/project_serializer.hpp"
#include <fstream>
#include <sstream>

namespace fif::core {
	ProjectSerializer::ProjectSerializer(const std::shared_ptr<Project> &project) : m_Project(project) {
	}

	void ProjectSerializer::serialize() {
		const std::filesystem::path projectFilePath = m_Project->get_project_file_path();
		Logger::info("Serializing project: %s", projectFilePath.c_str());

		YAML::Emitter yaml;
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "Config" << YAML::Value << YAML::BeginMap;
		yaml << YAML::Key << "Name" << YAML::Value << m_Project->m_Config.name;
		yaml << YAML::Key << "StartingScene" << YAML::Value << m_Project->m_Config.startingScenePath;
		yaml << YAML::EndMap;
		yaml << YAML::EndMap;

		std::ofstream fileStream(projectFilePath);
		fileStream << yaml.c_str();
	}

	bool ProjectSerializer::deserialize() {
		const std::filesystem::path projectFilePath = m_Project->get_project_file_path();
		Logger::info("Deserializing project: %s", projectFilePath.c_str());

		if(!std::filesystem::exists(projectFilePath)) {
			Logger::error("Project file '%s' doesn't exist!", projectFilePath.c_str());
			return false;
		}

		std::ifstream fileStream(projectFilePath);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		const YAML::Node rootNode = YAML::Load(ss.str());
		const YAML::Node configNode = rootNode["Config"];

		FIF_ASSERT(static_cast<bool>(configNode), "Project file is invalid, Config map is missing!");

		m_Project->m_Config.name = configNode["Name"].as<std::string>();
		m_Project->m_Config.startingScenePath = configNode["StartingScene"].as<std::string>();

		return true;
	}
}// namespace fif::core
