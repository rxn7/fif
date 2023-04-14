#include "fif/gfx/shaderLibrary.h"
#include "fif/core/assertion.h"

#include "shaders/circle.h"
#include "shaders/quad.h"

#include <memory>

namespace fif::gfx {
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;

	void ShaderLibrary::init() {
		add("circle", std::make_shared<Shader>(shaders::circle::VERTEX, shaders::circle::FRAGMENT));
		add("quad", std::make_shared<Shader>(shaders::quad::VERTEX, shaders::quad::FRAGMENT));
	}

	std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name) {
		auto it = s_Shaders.find(name);
		FIF_ASSERT(it != s_Shaders.end(), "Shader with name '" << name << "' doesn't exists");
		return it->second;
	}

	void ShaderLibrary::add(const std::string &name, const std::shared_ptr<Shader> &shader) {
		FIF_ASSERT(shader.get() != nullptr, "The shader cannot be nullptr");
		s_Shaders.insert({name, shader});
	}
}
