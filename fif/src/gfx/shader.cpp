#include "fif/gfx/shader.hpp"

namespace fif::gfx {
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc, const std::initializer_list<std::string_view> uniforms) {
		m_ID = glCreateProgram();

		const u32 fragID = compile(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
		const u32 vertID = compile(GL_VERTEX_SHADER, vertexSrc.c_str());

		glAttachShader(m_ID, fragID);
		glAttachShader(m_ID, vertID);

		glLinkProgram(m_ID);
		if(check_status(m_ID, GL_LINK_STATUS, true)) {
			glValidateProgram(m_ID);
			m_Valid = check_status(m_ID, GL_VALIDATE_STATUS, true);
		}

		glDeleteShader(fragID);
		glDeleteShader(vertID);

		if(!m_Valid) {
			core::Logger::error("Shader is invalid: %s", glGetError());
			return;
		}

		for(std::string_view uniform : uniforms) {
			const i32 location = glGetUniformLocation(m_ID, uniform.data());
			if(location == -1) {
				core::Logger::error("Uniform '%s' doesn't exist.", uniform.data());
				continue;
			}

			m_UniformLocations.insert({uniform, location});
		}
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

	u32 Shader::compile(GLenum type, const char *src) {
		FIF_ASSERT(type == GL_FRAGMENT_SHADER || type == GL_VERTEX_SHADER, "Only fragment and vertex shaders are supported");
		const u32 id = glCreateShader(type);

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);
		check_status(id, GL_COMPILE_STATUS, false);

		return id;
	}

	i32 Shader::get_uniform_location(std::string_view name) {
		const auto it = m_UniformLocations.find(name);

		if(it == m_UniformLocations.end()) {
			const i32 location = glGetUniformLocation(m_ID, name.data());
			if(location == -1)
				return -1;

			return m_UniformLocations.insert({name, location}).second;
		}

		return it->second;
	}

	bool Shader::check_status(u32 id, GLenum type, bool program) {
		i32 status;

		if(program)
			glGetProgramiv(id, type, &status);
		else
			glGetShaderiv(id, type, &status);

		if(!status)
			print_info_log(id, program);

		return static_cast<bool>(status);
	}

	void Shader::print_info_log(u32 id, bool program) {
		int logLength;

		if(program)
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		else
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		char *buffer = new char[logLength + 1];

		if(program)
			glGetProgramInfoLog(id, logLength, NULL, buffer);
		else
			glGetShaderInfoLog(id, logLength, NULL, buffer);

		core::Logger::error("Shader error: %s", buffer);

		delete[] buffer;
	}
}// namespace fif::gfx
