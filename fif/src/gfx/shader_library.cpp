#include "fif/gfx/shader_library.hpp"
#include "fif/core/util/assertion.hpp"
#include "shaders/circle.hpp"
#include "shaders/quad.hpp"
#include "shaders/sprite.hpp"

namespace fif::gfx {
	std::unordered_map<std::string_view, std::shared_ptr<Shader>> ShaderLibrary::s_Shaders;

	std::shared_ptr<Shader> ShaderLibrary::get(std::string_view name) {
		auto it = s_Shaders.find(name);
		FIF_ASSERT(it != s_Shaders.end(), "Shader with name '%s' doesn't exists", name.data());
		return it->second;
	}

	std::shared_ptr<Shader> ShaderLibrary::add(std::string_view name, const std::string &vertexSrc, const std::string &fragSrc) {
		std::shared_ptr<Shader> p_shader = std::make_shared<Shader>(vertexSrc, fragSrc);
		if(!p_shader->is_valid()) {
			core::Logger::error("Cannot add invalid shader to ShaderLibrary!");
			return nullptr;
		}

		auto result = s_Shaders.insert({name, p_shader});
		return result.first->second;
	}
}// namespace fif::gfx
