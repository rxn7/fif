#include "fif/core/serialization/vec3_yaml.hpp"

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::vec3 &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
}// namespace YAML
