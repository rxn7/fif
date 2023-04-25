#include "fif/gfx/shader_library.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/gfx/shaders/circle.hpp"
#include "fif/gfx/shaders/simple.hpp"

#include <memory>

namespace fif::gfx {
	static std::unordered_map<std::string, std::unique_ptr<Shader>> s_Shaders;

	void ShaderLibrary::init() {
		add("circle", shaders::Simple::VERTEX, shaders::Circle::FRAGMENT);
		add("simple", shaders::Simple::VERTEX, shaders::Simple::FRAGMENT);
	}

	Shader &ShaderLibrary::get(const std::string &name) {
		auto it = s_Shaders.find(name);
		FIF_ASSERT(it != s_Shaders.end(), "Shader with name '" << name << "' doesn't exists");
		return *it->second;
	}

	Shader &ShaderLibrary::add(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
		auto result = s_Shaders.insert({name, std::make_unique<Shader>(vertexSrc, fragmentSrc)});
		return *result.first->second;
	}
}// namespace fif::gfx
