#pragma once

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/node.h"

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::vec3 &v);

	template<> struct convert<fif::vec3> {
		static Node encode(const fif::vec3 &v) {
			Node n;
			n.push_back(v.x);
			n.push_back(v.y);
			n.push_back(v.z);
			return n;
		}

		static bool decode(const Node &node, fif::vec3 &v) {
			if(!node.IsSequence() || node.size() != 3)
				return false;
			v.x = node[0].as<f32>();
			v.y = node[1].as<f32>();
			v.z = node[2].as<f32>();
			return true;
		}
	};
}// namespace YAML
