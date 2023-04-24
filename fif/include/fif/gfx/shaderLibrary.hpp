#pragma once

#include "fif/gfx/shader.hpp"

#include <initializer_list>
#include <memory>

namespace fif::gfx {
	namespace ShaderLibrary {
		void init();
		Shader &add(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
		Shader &get(const std::string &name);
	}; // namespace ShaderLibrary
} // namespace fif::gfx
