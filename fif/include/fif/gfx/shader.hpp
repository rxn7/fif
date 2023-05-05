#pragma once

#include "fif/gfx/color.hpp"

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
		static void unbind();

		inline void set_uniform(const std::string &name, int value) { glUniform1i(get_uniform_location(name), value); }

		inline void set_uniform(const std::string &name, f32 value) { glUniform1f(get_uniform_location(name), value); }

		inline void set_uniform(const std::string &name, const Color &value) { set_uniform(name, fif::gfx::get_normalized_color(value)); }

		inline void set_uniform(const std::string &name, const glm::vec2 &value) { glUniform2f(get_uniform_location(name), value.x, value.y); }

		inline void set_uniform(const std::string &name, const glm::vec3 &value) {
			glUniform3f(get_uniform_location(name), value.x, value.y, value.z);
		}

		inline void set_uniform(const std::string &name, const glm::vec4 &value) {
			glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
		}

		inline void set_uniform(const std::string &name, const glm::mat3 &value) {
			glUniformMatrix3fv(get_uniform_location(name), 1, 0u, glm::value_ptr(value));
		}

		inline void set_uniform(const std::string &name, const glm::mat4 &value) {
			glUniformMatrix4fv(get_uniform_location(name), 1, 0u, glm::value_ptr(value));
		}

	private:
		u32 get_uniform_location(const std::string &name);

		static u32 compile(GLenum type, const char *src);
		static void check_status(u32 id, GLenum type, bool program = true);
		static void print_info_log(u32 id, bool program = true);

	private:
		std::unordered_map<std::string, u32> m_UniformIDs;
		u32 m_ID;
	};
}// namespace fif::gfx
