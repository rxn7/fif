#include "fif/core/ecs/serialization/vec2_yaml.hpp"

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::vec2 &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
}// namespace YAML
