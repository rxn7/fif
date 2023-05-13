#pragma once

#include "fif/gfx/shader.hpp"

namespace fif::gfx {
	// TODO: Change this to AssetManager
	class ShaderLibrary final {
	public:
		static std::shared_ptr<Shader> get(std::string_view name);
		static std::shared_ptr<Shader> add(std::string_view name, const std::string &vertexSrc, const std::string &fragSrc);

	private:
		static std::unordered_map<std::string_view, std::shared_ptr<Shader>> s_Shaders;
	};
}// namespace fif::gfx
