#include "fif/gfx/shader.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/core/util/log.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <array>

namespace fif::gfx {
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
		m_ID = glCreateProgram();

		const u32 fragID = compile(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
		const u32 vertID = compile(GL_VERTEX_SHADER, vertexSrc.c_str());

		glAttachShader(m_ID, fragID);
		glAttachShader(m_ID, vertID);

		glLinkProgram(m_ID);

		const auto printInfoLog = [](u32 id) {
			int logLength;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

			char *buffer = new char[logLength];
			glGetProgramInfoLog(id, logLength, NULL, buffer);
			FIF_LOG_ERROR("Failed to link shader program: " << buffer);

			delete[] buffer;
		};

		int status;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &status);
		if(status == 0) {
			printInfoLog(m_ID);
		}

		glValidateProgram(m_ID);
		glGetProgramiv(m_ID, GL_VALIDATE_STATUS, &status);
		if(status == 0) {
			printInfoLog(m_ID);
		}

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

	u32 Shader::compile(int type, const char *src) {
		FIF_ASSERT(type == GL_FRAGMENT_SHADER || type == GL_VERTEX_SHADER, "Only fragment and vertex shaders are supported");
		const u32 id = glCreateShader(type);

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		int status;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if(status == 0) {
			int logLength;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

			char *buffer = new char[logLength];
			glGetShaderInfoLog(id, logLength, NULL, buffer);

			FIF_LOG_ERROR("Failed to compile shader: " << buffer);

			delete[] buffer;
		}

		return id;
	}

	void Shader::register_uniform(const std::string &name) {
		const auto it = m_UniformIDs.find(name);
		if(it != m_UniformIDs.end()) {
			FIF_LOG_ERROR("Uniform " << name << " already registered");
			return;
		}

		const u32 id = glGetUniformLocation(m_ID, name.c_str());
		m_UniformIDs.insert({name, id});
	}

	u32 Shader::get_uniform_location(const std::string &name) const {
		const auto it = m_UniformIDs.find(name);
		if(it == m_UniformIDs.end()) {
			FIF_LOG_ERROR("Uniform " << name << " not found");
			return 0;
		}
		return it->second;
	}

	void Shader::set_uniform(const std::string &name, int value) const {
		glUniform1i(get_uniform_location(name), value);
	}

	void Shader::set_uniform(const std::string &name, float value) const {
		glUniform1f(get_uniform_location(name), value);
	}

	void Shader::set_uniform(const std::string &name, const glm::vec2 &value) const {
		glUniform2f(get_uniform_location(name), value.x, value.y);
	}

	void Shader::set_uniform(const std::string &name, const glm::vec3 &value) const {
		glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
	}

	void Shader::set_uniform(const std::string &name, const glm::vec4 &value) const {
		glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
	}

	void Shader::set_uniform(const std::string &name, const glm::mat3 &value) const {
		glUniformMatrix3fv(get_uniform_location(name), 1, 0u, glm::value_ptr(value));
	}

	void Shader::set_uniform(const std::string &name, const glm::mat4 &value) const {
		glUniformMatrix4fv(get_uniform_location(name), 1, 0u, glm::value_ptr(value));
	}
}// namespace fif::gfx
