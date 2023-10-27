#include "fif/core/ecs/serialization/scene_serializer.hpp"

#include <fstream>
#include <sstream>

namespace fif::core {
	std::vector<std::unique_ptr<Serializer>> SceneSerializer::s_Serializers;

	SceneSerializer::SceneSerializer(Scene &scene) : m_Scene(scene) {
	}

	void SceneSerializer::serialize(const std::string &path) {
		Logger::info("Serializing scene: %s", path.c_str());

		YAML::Emitter yaml;
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene.for_each([&](EntityID id) {
			Entity entity(m_Scene, id);

			yaml << YAML::BeginMap;
			yaml << YAML::Key << "UUID" << YAML::Value << static_cast<u32>(id);// TODO: UUID

			for(const auto &serializer : s_Serializers) {
				serializer->serialize(entity, yaml);
			}

			yaml << YAML::EndMap;
		});

		yaml << YAML::EndSeq;
		yaml << YAML::EndMap;

		std::ofstream fileStream(path);
		fileStream << yaml.c_str();
	}

	void SceneSerializer::deserialize(const std::string &path) {
		m_Scene.clear();
		Logger::info("Deserializing scene: %s", path.c_str());

		std::ifstream fileStream(path);
		std::stringstream ss;
		ss << fileStream.rdbuf();

		const YAML::Node rootNode = YAML::Load(ss.str());

		const YAML::Node entitiesNode = rootNode["Entities"];
		if(!entitiesNode)
			return;

		for(const YAML::Node &entityNode : entitiesNode) {
			Entity entity(m_Scene, m_Scene.get_registry().create());

			for(const auto &serializer : s_Serializers) {
				serializer->deserialize(entity, entityNode);
			}
		}
	}
}// namespace fif::core

namespace YAML {
	void handle_exception(const char *what) {
		fif::core::Logger::error("YAML exception: %s", what);
		std::terminate();
	}
}// namespace YAML
