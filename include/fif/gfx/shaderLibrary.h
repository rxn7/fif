#pragma once

#include "fif/gfx/shader.h"

#include <memory>
#include <initializer_list>

namespace fif::gfx {
	namespace ShaderLibrary {
		void init();
		Shader &add(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
		Shader &get(const std::string &name);
	};
}
