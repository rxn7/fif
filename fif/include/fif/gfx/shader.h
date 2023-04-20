#pragma once

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
		void registerUniform(const std::string &name);
		void setUniform(const std::string &name, int value) const;
		void setUniform(const std::string &name, float value) const;
		void setUniform(const std::string &name, const glm::vec2 &value) const;
		void setUniform(const std::string &name, const glm::vec3 &value) const;
		void setUniform(const std::string &name, const glm::vec4 &value) const;
		void setUniform(const std::string &name, const glm::mat3 &value) const;
		void setUniform(const std::string &name, const glm::mat4 &value) const;
		static void unbind();

	  public:
	  private:
		std::uint32_t getUniformLocation(const std::string &name) const;
		static std::uint32_t compile(int type, const char *src);

	  private:
		std::unordered_map<std::string, std::uint32_t> m_UniformIDs;
		std::uint32_t m_ID;
	};
} // namespace fif::gfx
