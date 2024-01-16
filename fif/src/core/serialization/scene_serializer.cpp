#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/core/project.hpp"

#include <fstream>
#include <sstream>

namespace fif::core {
	SceneSerializer::SceneSerializer(Scene &scene) : m_Scene(scene) {}

	bool SceneSerializer::serialize(const std::filesystem::path &path) {
		const std::filesystem::path resourcePath = Project::get_resource_path(path);
		Logger::debug("Serializing scene: %s", path.c_str());

		YAML::Emitter yaml;
		yaml << YAML::BeginMap;
		yaml << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		// NOTE: We have to read it backwards, otherwise the order gets flipped (possible bug in entt)
		for(auto it = m_Scene.get_registry().storage<EntityID>().rbegin(); it != m_Scene.get_registry().storage<EntityID>().rend(); ++it) {
			Entity entity(m_Scene, *it);

			yaml << YAML::BeginMap;

			for(const auto &serializer : s_Serializers)
				serializer->serialize(entity, yaml);

			yaml << YAML::EndMap;
		}

		yaml << YAML::EndSeq;
		yaml << YAML::EndMap;

		std::ofstream fileStream(resourcePath);
		fileStream << yaml.c_str();

		return true;
	}

	bool SceneSerializer::deserialize(const std::filesystem::path &path) {
		const std::filesystem::path resourcePath = Project::get_resource_path(path);
		Logger::debug("Deserializing scene: %s", path.c_str());

		if(!std::filesystem::exists(resourcePath)) {
			Logger::error("Scene '%s' doesn't exist!", path.c_str());
			return false;
		}

		m_Scene.clear();

		std::ifstream fileStream(resourcePath);
		std::ostringstream ss;
		ss << fileStream.rdbuf();

		const YAML::Node rootNode = YAML::Load(ss.str());
		if(const YAML::Node entitiesNode = rootNode["Entities"]) {
			for(const YAML::Node &entityNode : entitiesNode) {
				Entity entity(m_Scene, m_Scene.get_registry().create());

				for(const auto &serializer : s_Serializers)
					serializer->deserialize(entity, entityNode);
			}
		}

		return true;
	}
}// namespace fif::core

namespace YAML {
	void handle_exception(const char *what) {
		fif::core::Logger::error("YAML exception: %s", what);
		std::terminate();
	}
}// namespace YAML
