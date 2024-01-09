#include "fif/core/resource/resource_manager.hpp"
#include "fif/core/project.hpp"
#include "fif/core/resource/resource.hpp"

// TODO: Somehow add metadata to the resources. (i.e. font size, texture filter)

namespace fif::core {
	ResourceManager::~ResourceManager() { Logger::debug("ResourceManager::dtor"); }

	void ResourceManager::serialize(YAML::Emitter &yaml) const {
		yaml << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for(const auto &entry : m_Resources) {
			yaml << YAML::BeginMap;
			yaml << YAML::Key << "UUID" << entry.second->get_uuid();
			yaml << YAML::Key << "Path" << entry.second->get_path().string();
			yaml << YAML::Key << "Type" << entry.second->get_type().c_str();
			yaml << YAML::EndMap;
		}

		yaml << YAML::EndSeq;
	}

	void ResourceManager::deserialize(const YAML::Node &projectResourcesNode) {
		if(!projectResourcesNode)
			return;

		for(const YAML::Node &resourceNode : projectResourcesNode) {
			const std::filesystem::path resourcePath = resourceNode["Path"].as<std::string>();
			const std::string resourceType = resourceNode["Type"].as<std::string>();

			if(!std::filesystem::exists(Project::get_resource_path(resourcePath))) {
				Logger::error("Resource '%s' of type '%s' doesn't exist", resourcePath.string().c_str(), resourceType.c_str());
				continue;
			}

			const ResourceLoaderFunc loader = get_loader_func(resourceType);
			if(!loader)
				continue;

			std::shared_ptr<Resource> loadedResource = loader(resourcePath);

			const UUID resourceUuid = resourceNode["UUID"].as<u64>();
			loadedResource->m_UUID = resourceUuid;

			m_Resources.insert({resourceUuid, loadedResource});

			Logger::debug("Resource '%s' of type '%s' has been loaded", resourcePath.string().c_str(), resourceType.c_str());
		}
	}

	void ResourceManager::clear() {
		m_Resources.clear();
		Logger::debug("ResourceManager cleared");
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
