#pragma once

#include "fif/core/serialization/serializer.hpp"

#include "yaml-cpp/yaml.h"
#include <filesystem>
#include <type_traits>
#include <vector>

namespace fif::core {
	class SceneSerializer {
	public:
		SceneSerializer(Scene &scene);
		void deserialize(std::filesystem::path path);
		void serialize(std::filesystem::path path);

		template<class T> static void add_serializer() {
			static_assert(std::is_base_of<Serializer, T>().value, "T doesn't derive from Serializer!");
			s_Serializers.emplace_back(std::make_unique<T>());
		}

	private:
		static std::vector<std::unique_ptr<Serializer>> s_Serializers;
		Scene &m_Scene;
	};
}// namespace fif::core
