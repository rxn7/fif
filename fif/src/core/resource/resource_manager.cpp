#include "fif/core/resource/resource_manager.hpp"
#include "fif/core/resource/resource.hpp"

namespace fif::core {
	void ResourceManager::serialize(YAML::Emitter &yaml) const {
		yaml << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for(const auto &entry : m_Resources) {
			yaml << YAML::BeginMap;
			yaml << YAML::Key << "UUID" << entry.second->get_uuid();
			yaml << YAML::Key << "Path" << entry.second->get_path();
			yaml << YAML::Key << "Type" << entry.second->get_type().c_str();
			yaml << YAML::EndMap;
		}

		yaml << YAML::EndSeq;
	}

	void ResourceManager::deserialize(const YAML::Node &projectResourcesNode) {
		if(!projectResourcesNode)
			return;

		for(const YAML::Node &resourceNode : projectResourcesNode) {
			const std::string resourceType = resourceNode["Type"].as<std::string>();

			const ResourceLoaderFunc loader = get_loader_func(resourceType);
			if(!loader)
				continue;

			const std::filesystem::path resourcePath = resourceNode["Path"].as<std::string>();
			const UUID resourceUuid = resourceNode["UUID"].as<u64>();

			std::shared_ptr<Resource> loadedResource = loader(resourcePath);
			loadedResource->m_UUID = resourceUuid;

			m_Resources.insert({resourceUuid, loadedResource});

			Logger::debug("Resource '%s' of type '%s' has been loaded", resourcePath.string().c_str(), resourceType.c_str());
		}
	}

	void ResourceManager::add_loader_func(const std::string &type, const ResourceLoaderFunc loader) {
		if(s_Loaders.find(type) != s_Loaders.end()) {
			Logger::error("Resource loader of type '%s' already exist", type.c_str());
			return;
		}

		s_Loaders.insert({type, loader});
	}

	ResourceLoaderFunc ResourceManager::get_loader_func(const std::string &type) {
		const auto &it = s_Loaders.find(type);
		if(it == s_Loaders.end()) {
			Logger::error("Resource loader of type '%s' doesn't exist", type.c_str());
			return nullptr;
		}
		return it->second;
	}
}// namespace fif::core
