#pragma once

#include <yaml-cpp/yaml.h>

namespace fif::core {
	class SceneSerializer {
	public:
		SceneSerializer(Scene &scene);
		void deserialize(const std::string &path);
		void serialize(const std::string &path);

	private:
		Scene &m_Scene;
	};
}// namespace fif::core
