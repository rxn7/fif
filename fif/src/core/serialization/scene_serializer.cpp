#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/core/project.hpp"

#include <fstream>
#include <sstream>

namespace fif::core {
	SceneSerializer::SceneSerializer(Scene &scene) : m_Scene(scene) {
	}

	void SceneSerializer::serialize(const std::filesystem::path &path) {
		const std::filesystem::path resourcePath = Project::get_resource_path(path);
		Logger::info("Serializing scene: %s", path.c_str());

		YAML::Emitter yaml;
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene.for_each([&](EntityID id) {
			Entity entity(m_Scene, id);

			yaml << YAML::BeginMap;

			for(const auto &serializer : s_Serializers)
				serializer->serialize(entity, yaml);

			yaml << YAML::EndMap;
		});

		yaml << YAML::EndSeq;
		yaml << YAML::EndMap;

		std::ofstream fileStream(resourcePath);
		fileStream << yaml.c_str();
	}

	void SceneSerializer::deserialize(const std::filesystem::path &path) {
		const std::filesystem::path resourcePath = Project::get_resource_path(path);
		Logger::info("Deserializing scene: %s", path.c_str());

		m_Scene.clear();

		std::ifstream fileStream(resourcePath);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		const YAML::Node rootNode = YAML::Load(ss.str());

		const YAML::Node entitiesNode = rootNode["Entities"];
		if(!entitiesNode)
			return;

		for(const YAML::Node &entityNode : entitiesNode) {
			Entity entity(m_Scene, m_Scene.get_registry().create());

			for(const auto &serializer : s_Serializers)
				serializer->deserialize(entity, entityNode);
		}
	}
}// namespace fif::core

namespace YAML {
	void handle_exception(const char *what) {
		fif::core::Logger::error("YAML exception: %s", what);
		std::terminate();
	}
}// namespace YAML
