#pragma once

#include <memory>
#include "fif/gfx/shader.h"

namespace fif::gfx {
	namespace ShaderLibrary {
		void init();
		std::shared_ptr<Shader> add(const std::string &name, const std::shared_ptr<Shader> &shader);
		std::shared_ptr<Shader> get(const std::string &name);
	};
}
