#pragma once

#include "fif/core/serialization/entity_serializer.hpp"

namespace fif::gfx {
	class GfxEntitySerializer : public core::EntitySerializer {
	public:
		void serialize(core::Entity &entity, YAML::Emitter &emitter) override;
		void deserialize(core::Entity &entity, const YAML::Node &entityNode) override;
	};
}// namespace fif::gfx
