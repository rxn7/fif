#pragma once

#include "fif/core/ecs/entity.hpp"

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/node.h"

namespace fif::core {
	class EntitySerializer {
	public:
		virtual void serialize(Entity &entity, YAML::Emitter &yaml) = 0;
		virtual void deserialize(Entity &entity, const YAML::Node &entityNode) = 0;

	protected:
		template<typename T> static void try_get_component_node(const YAML::Node &entityNode, std::function<void(const YAML::Node &node)> func) {
			const YAML::Node componentNode = entityNode[std::string(entt::type_id<T>().name())];
			if(componentNode)
				func(componentNode);
		}

		template<typename T> static inline void serialize_component(const Entity &entity, YAML::Emitter &yaml, std::function<void(T &t)> func) {
			if(entity.has_component<T>()) {
				yaml << YAML::Key << std::string(entt::type_id<T>().name());
				yaml << YAML::BeginMap;
				func(entity.get_component<T>());
				yaml << YAML::EndMap;
			}
		}
	};
}// namespace fif::core
