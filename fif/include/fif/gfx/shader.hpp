#pragma once

#include "fif/gfx/color.hpp"

#define FIF_GLSL_VERSION "#version 300 es\n"
#define FIF_GLSL_PRECISION "precision mediump float;\n"

namespace fif::gfx {
	class Shader final {
	public:
		Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
		~Shader();

		void bind() const;

		static void unbind();

		inline bool is_valid() const { return m_Valid; }

		inline void set_uniform_i32(const std::string &name, i32 value) { glUniform1i(get_uniform_location(name), value); }

		inline void set_uniform_i32_array(const std::string &name, i32 values[], u32 count) { glUniform1iv(get_uniform_location(name), count, values); }

		inline void set_uniform_f32(const std::string &name, f32 value) { glUniform1f(get_uniform_location(name), value); }

		inline void set_uniform_color(const std::string &name, const Color &value) { set_uniform_vec3(name, fif::gfx::denormalize_color3(value)); }

		inline void set_uniform_vec2(const std::string &name, const vec2 &value) { glUniform2f(get_uniform_location(name), value.x, value.y); }

		inline void set_uniform_vec3(const std::string &name, const vec3 &value) { glUniform3f(get_uniform_location(name), value.x, value.y, value.z); }

		inline void set_uniform_vec4(const std::string &name, const vec4 &value) { glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w); }

		inline void set_uniform_mat3(const std::string &name, const mat3 &value) { glUniformMatrix3fv(get_uniform_location(name), 1, 0u, value_ptr(value)); }

		inline void set_uniform_mat4(const std::string &name, const mat4 &value) { glUniformMatrix4fv(get_uniform_location(name), 1, 0u, value_ptr(value)); }

	private:
		i32 get_uniform_location(const std::string &name);

		static u32 compile(const GLenum type, const char *src);
		static bool check_status(const u32 id, const GLenum type, const bool program = true);
		static void print_info_log(const u32 id, const bool program = true);

	private:
		bool m_Valid = false;
		std::unordered_map<std::string, i32> m_UniformLocations;
		u32 m_ID = 0;
	};
}// namespace fif::gfx
