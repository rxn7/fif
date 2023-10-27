#pragma once

#include "fif/core/ecs/serialization/serializer.hpp"

#include "yaml-cpp/yaml.h"
#include <type_traits>
#include <vector>

namespace fif::core {
	class SceneSerializer {
	public:
		SceneSerializer(Scene &scene);
		void deserialize(const std::string &path);
		void serialize(const std::string &path);

		template<class T> static void add_serializer() {
			static_assert(std::is_base_of<Serializer, T>().value, "T doesn't derive from Serializer!");
			s_Serializers.emplace_back(std::make_unique<T>());
		}

	private:
		Scene &m_Scene;
		static std::vector<std::unique_ptr<Serializer>> s_Serializers;
	};
}// namespace fif::core
