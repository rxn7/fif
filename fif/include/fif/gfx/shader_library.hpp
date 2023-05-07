#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx {
	namespace ShaderLibrary {
		std::shared_ptr<Shader> add(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
		std::shared_ptr<Shader> get(const std::string &name);
	};// namespace ShaderLibrary
}// namespace fif::gfx
