#include "fif/gfx/shader_library.hpp"
#include "fif/core/util/assertion.hpp"
#include "shaders/circle.hpp"
#include "shaders/quad.hpp"
#include "shaders/sprite.hpp"

namespace fif::gfx {
	static std::unordered_map<std::string, std::shared_ptr<Shader>> s_Shaders;

	std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name) {
		auto it = s_Shaders.find(name);
		FIF_ASSERT(it != s_Shaders.end(), "Shader with name '" << name << "' doesn't exists");
		return it->second;
	}

	std::shared_ptr<Shader> ShaderLibrary::add(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
		auto result = s_Shaders.insert({name, std::make_shared<Shader>(vertexSrc, fragmentSrc)});
		return result.first->second;
	}
}// namespace fif::gfx
