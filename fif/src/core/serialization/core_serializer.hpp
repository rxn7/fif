#pragma once

#include "fif/core/serialization/serializer.hpp"

namespace fif::core {
	class CoreSerializer : public Serializer {
	public:
		void serialize(const core::Entity &entity, YAML::Emitter &emitter) override;
		void deserialize(core::Entity &entity, const YAML::Node &entityNode) override;
	};
}// namespace fif::core
