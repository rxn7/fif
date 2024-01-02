#pragma once

#include "fif/core/serialization/serializer.hpp"

namespace fif::gfx {
	class GfxSerializer : public core::Serializer {
	public:
		void serialize(const core::Entity &entity, YAML::Emitter &emitter) override;
		void deserialize(core::Entity &entity, const YAML::Node &entityNode) override;
	};
}// namespace fif::gfx
