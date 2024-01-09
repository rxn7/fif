#pragma once

#include "fif/core/resource/resource.hpp"

#include <unordered_map>

namespace fif::core {
	typedef std::function<std::shared_ptr<Resource>(const std::filesystem::path &)> ResourceLoaderFunc;

	class Resource;

	class ResourceManager final {
	public:
		~ResourceManager();
		void serialize(YAML::Emitter &yaml) const;
		void deserialize(const YAML::Node &projectResourcesNode);

		template<typename T> std::shared_ptr<T> add_resource(const std::filesystem::path &path) {
			static_assert(std::is_base_of<Resource, T>(), "T must be derived from Resource");

			for(const auto &entry : m_Resources) {
				if(entry.second->get_path() == path) {
					Logger::debug("Resource %s already exists, so it was not loaded again", path.string().c_str());
					return std::dynamic_pointer_cast<T>(entry.second);
				}
			}

			const std::string type = T::get_type_static();
			const ResourceLoaderFunc loader = get_loader_func(type);
			if(!loader)
				return nullptr;

			const std::shared_ptr<T> resource = std::dynamic_pointer_cast<T>(loader(path));
			if(!resource) {
				Logger::error("Resource '%s' of type '%s' could not be loaded", path.string().c_str(), type.c_str());
				return nullptr;
			}

			m_Resources.insert({resource->get_uuid(), resource});

			return resource;
		}

		template<typename T> std::shared_ptr<T> get_resource(UUID uuid) {
			static_assert(std::is_base_of<Resource, T>(), "T must be derived from Resource");

			const auto &it = m_Resources.find(uuid);
			if(it == m_Resources.end()) {
				Logger::error("Resource '%llu' of type '%s' is not loaded", uuid, T::get_type_static().c_str());
				return nullptr;
			}

			return std::dynamic_pointer_cast<T>(it->second);
		}

		void clear();

		static void add_loader_func(const std::string &type, const ResourceLoaderFunc loader);
		ResourceLoaderFunc get_loader_func(const std::string &type);

	private:
		std::unordered_map<u64, std::shared_ptr<Resource>> m_Resources;
		static inline std::unordered_map<std::string, ResourceLoaderFunc> s_Loaders;
	};
}// namespace fif::core
