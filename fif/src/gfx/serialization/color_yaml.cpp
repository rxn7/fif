#include "fif/gfx/serialization/color_yaml.hpp"

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::gfx::Color &v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << static_cast<i32>(v.r) << static_cast<i32>(v.g) << static_cast<i32>(v.b) << static_cast<i32>(v.a) << YAML::EndSeq;
		return out;
	}
}// namespace YAML
