#pragma once

#include "fif/core/ecs/serialization/serializer.hpp"

namespace fif::core {
	class CoreSerializer : public core::Serializer {
	public:
		void serialize(const core::Entity &entity, YAML::Emitter &emitter) override;
		void deserialize(core::Entity &entity, const YAML::Node &entityNode) override;
	};
}// namespace fif::core
