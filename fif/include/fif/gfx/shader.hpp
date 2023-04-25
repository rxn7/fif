#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/color.hpp"

#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <memory>
#include <string>
#include <unordered_map>

#define FIF_GLSL_VERSION "#version 300 es\n"
#define FIF_GLSL_PRECISION "precision mediump float;\n"

namespace fif::gfx {
	class Shader final {
	public:
		Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
		~Shader();

		void bind() const;
		u32 register_uniform(const std::string &name);
		void set_uniform(const std::string &name, i32 value);
		void set_uniform(const std::string &name, f32 value);
		void set_uniform(const std::string &name, const Color &value);
		void set_uniform(const std::string &name, const glm::vec2 &value);
		void set_uniform(const std::string &name, const glm::vec3 &value);
		void set_uniform(const std::string &name, const glm::vec4 &value);
		void set_uniform(const std::string &name, const glm::mat3 &value);
		void set_uniform(const std::string &name, const glm::mat4 &value);
		static void unbind();

	public:
	private:
		u32 get_uniform_location(const std::string &name);
		static u32 compile(int type, const char *src);

	private:
		std::unordered_map<std::string, u32> m_UniformIDs;
		u32 m_ID;
	};
}// namespace fif::gfx
