#include "fif/gfx/shader.hpp"

namespace fif::gfx {
	Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
		m_ID = glCreateProgram();

		const u32 fragID = compile(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
		const u32 vertID = compile(GL_VERTEX_SHADER, vertexSrc.c_str());

		glAttachShader(m_ID, fragID);
		glAttachShader(m_ID, vertID);

		glLinkProgram(m_ID);
		check_status(m_ID, GL_LINK_STATUS, true);

		glValidateProgram(m_ID);
		check_status(m_ID, GL_VALIDATE_STATUS, true);

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

	u32 Shader::compile(GLenum type, const char *src) {
		FIF_ASSERT(type == GL_FRAGMENT_SHADER || type == GL_VERTEX_SHADER, "Only fragment and vertex shaders are supported");
		const u32 id = glCreateShader(type);

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);
		check_status(id, GL_COMPILE_STATUS, false);

		return id;
	}

	u32 Shader::register_uniform(const std::string &name) {
		const auto it = m_UniformIDs.find(name);
		if(it != m_UniformIDs.end()) {
			core::Logger::error("Uniform %s already registered", name.c_str());
			return it->second;
		}

		const u32 id = glGetUniformLocation(m_ID, name.c_str());
		m_UniformIDs.insert({name, id});

		return id;
	}

	u32 Shader::get_uniform_location(const std::string &name) {
		const auto it = m_UniformIDs.find(name);

		if(it == m_UniformIDs.end())
			return register_uniform(name);

		return it->second;
	}

	void Shader::check_status(u32 id, GLenum type, bool program) {
		i32 status;

		if(program)
			glGetProgramiv(id, type, &status);
		else
			glGetShaderiv(id, type, &status);

		if(!status)
			print_info_log(id, program);
	}

	void Shader::print_info_log(u32 id, bool program) {
		int logLength;

		if(program)
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
		else
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

		char buffer[logLength];

		if(program)
			glGetProgramInfoLog(id, logLength, NULL, buffer);
		else
			glGetShaderInfoLog(id, logLength, NULL, buffer);

		core::Logger::error("Shader erro: %s", buffer);
	}
}// namespace fif::gfx
