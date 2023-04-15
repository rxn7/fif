#include "fif/gfx/shader.h"
#include "fif/core/assertion.h"
#include "fif/core/log.h"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include <array>

namespace fif::gfx {
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
		m_ID = glCreateProgram();

		const std::uint32_t fragID = compile(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
		const std::uint32_t vertID = compile(GL_VERTEX_SHADER, vertexSrc.c_str());

		glAttachShader(m_ID, fragID);
		glAttachShader(m_ID, vertID);

		glLinkProgram(m_ID);

		const auto printInfoLog = [](std::uint32_t id) {
			int logLength;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

			char *buffer = new char[logLength];
			glGetProgramInfoLog(id, logLength, NULL, buffer);
			FIF_LOG_ERROR("Failed to link shader program: " << buffer);

			delete[] buffer;
		};

		int status;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &status);
		if(!status)
			printInfoLog(m_ID);
		
		glValidateProgram(m_ID);
		glGetProgramiv(m_ID, GL_VALIDATE_STATUS, &status);
		if(!status)
			printInfoLog(m_ID);

		glDeleteShader(fragID);
		glDeleteShader(vertID);
	}

	Shader::~Shader() {
		glDeleteProgram(m_ID);
	}

	void Shader::bind() const {
		glUseProgram(m_ID);
	}

	void Shader::unbind() {
		glUseProgram(0);
	}

	std::uint32_t Shader::compile(int type, const char *src) {
		FIF_ASSERT(type == GL_FRAGMENT_SHADER || type == GL_VERTEX_SHADER, "Only fragment and vertex shaders are supported");
		const std::uint32_t id = glCreateShader(type);

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		int status;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if(!status) {
			int logLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

			char *buffer = new char[logLength];
			glGetShaderInfoLog(id, logLength, NULL, buffer);

			FIF_LOG_ERROR("Failed to compile shader: " << buffer);

			delete[] buffer;
		}

		return id;
	}

	void Shader::registerUniform(const std::string &name) {
		const auto it = m_UniformIDs.find(name);
		if(it != m_UniformIDs.end()) {
			FIF_LOG_ERROR("Uniform " << name << " already registered");
			return;
		}

		const std::uint32_t id = glGetUniformLocation(m_ID, name.c_str());
		m_UniformIDs.insert({name, id});

		FIF_LOG("Uniform " << name << " registered");
	}

	std::uint32_t Shader::getUniformLocation(const std::string &name) const {
		const auto it = m_UniformIDs.find(name);
		if(it == m_UniformIDs.end()) {
			FIF_LOG_ERROR("Uniform " << name << " not found");
			return 0;
		}
		return it->second;
	}
	
	void Shader::setUniform(const std::string &name, int value) const {
		glUniform1i(getUniformLocation(name), value);
	}

	void Shader::setUniform(const std::string &name, float value) const {
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::setUniform(const std::string &name, glm::vec2 value) const {
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}

	void Shader::setUniform(const std::string &name, glm::vec3 value) const {
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::setUniform(const std::string &name, glm::vec4 value) const {
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::setUniform(const std::string &name, glm::mat3 value) const {
		glUniformMatrix3fv(getUniformLocation(name), value.length(), false, glm::value_ptr(value));
	}

	void Shader::setUniform(const std::string &name, glm::mat4 value) const {
		glUniformMatrix4fv(getUniformLocation(name), value.length(), false, glm::value_ptr(value));
	}
}
