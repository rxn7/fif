#pragma once

#include "fif/core/serialization/entity_serializer.hpp"

#include <filesystem>
#include <type_traits>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace fif::core {
	class SceneSerializer final {
	public:
		SceneSerializer(Scene &scene);
		void deserialize(const std::filesystem::path &path);
		void serialize(const std::filesystem::path &path);

		template<class T> static void add_serializer() {
			static_assert(std::is_base_of<EntitySerializer, T>().value, "T doesn't derive from Serializer!");
			s_Serializers.emplace_back(std::make_unique<T>());
		}

	private:
		inline static std::vector<std::unique_ptr<EntitySerializer>> s_Serializers;
		Scene &m_Scene;
	};
}// namespace fif::core
