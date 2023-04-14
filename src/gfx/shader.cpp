#include "fif/gfx/shader.h"
#include "fif/core/assertion.h"
#include "fif/core/log.h"

#include "glad/glad.h"

#include <array>

namespace fif::gfx {
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
		m_ID = glCreateProgram();

		std::uint32_t fragID = compile(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
		std::uint32_t vertID = compile(GL_VERTEX_SHADER, vertexSrc.c_str());

		glAttachShader(m_ID, fragID);
		glAttachShader(m_ID, vertID);

		glLinkProgram(m_ID);

		auto printInfoLog = [](std::uint32_t id) {
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
		std::uint32_t id = glCreateShader(type);

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
}
