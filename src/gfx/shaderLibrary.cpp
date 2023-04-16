#include "fif/gfx/shaderLibrary.h"
#include "fif/core/assertion.h"

#include "shaders/circle.h"
#include "shaders/simple.h"
#include <memory>

namespace fif::gfx {
	static std::unordered_map<std::string, std::unique_ptr<Shader>> s_Shaders;

	void ShaderLibrary::init() {
		{
			Shader &shader = add("circle", priv::shaders::Simple::VERTEX, priv::shaders::Circle::FRAGMENT);
			shader.registerUniform("u_MVP");
		}
		{
			Shader &shader = add("simple", priv::shaders::Simple::VERTEX, priv::shaders::Simple::FRAGMENT);
			shader.registerUniform("u_MVP");
		}
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
}
