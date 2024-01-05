#pragma once

#include "fif/gfx/color.hpp"

#include <yaml-cpp/emitter.h>
#include <yaml-cpp/node/node.h>

namespace YAML {
	Emitter &operator<<(Emitter &out, const fif::gfx::Color &v);

	template<> struct convert<fif::gfx::Color> {
		static Node encode(const fif::gfx::Color &v) {
			Node n;
			n.push_back(v.r);
			n.push_back(v.g);
			n.push_back(v.b);
			n.push_back(v.a);
			return n;
		}

		static bool decode(const Node &node, fif::gfx::Color &v) {
			if(!node.IsSequence() || node.size() != 4)
				return false;

			v.r = node[0].as<u8>();
			v.g = node[1].as<u8>();
			v.b = node[2].as<u8>();
			v.a = node[3].as<u8>();

			return true;
		}
	};
}// namespace YAML
