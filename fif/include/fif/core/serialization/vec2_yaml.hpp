#pragma once

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/node.h>

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::vec2 &v);

	template<> struct convert<fif::vec2> {
		static Node encode(const fif::vec2 &v) {
			Node n;
			n.push_back(v.x);
			n.push_back(v.y);
			return n;
		}

		static bool decode(const Node &node, fif::vec2 &v) {
			if(!node.IsSequence() || node.size() != 2)
				return false;
			v.x = node[0].as<f32>();
			v.y = node[1].as<f32>();
			return true;
		}
	};
}// namespace YAML
