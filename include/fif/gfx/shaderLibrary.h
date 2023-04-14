#pragma once

#include <memory>
#include "fif/gfx/shader.h"

namespace fif::gfx {
	class ShaderLibrary {
	public:
		static void init();
		static void add(const std::string &name, const std::shared_ptr<Shader> &shader);
		static std::shared_ptr<Shader> get(const std::string &name);
	};
}
