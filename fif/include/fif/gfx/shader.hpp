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

		inline void set_uniform_i32(std::string_view name, i32 value) { glUniform1i(get_uniform_location(name), value); }

		inline void set_uniform_i32_array(std::string_view name, i32 values[], u32 count) { glUniform1iv(get_uniform_location(name), count, values); }

		inline void set_uniform_f32(std::string_view name, f32 value) { glUniform1f(get_uniform_location(name), value); }

		inline void set_uniform_color(std::string_view name, const Color &value) { set_uniform_vec3(name, fif::gfx::denormalize_color3(value)); }

		inline void set_uniform_vec2(std::string_view name, const vec2 &value) { glUniform2f(get_uniform_location(name), value.x, value.y); }

		inline void set_uniform_vec3(std::string_view name, const vec3 &value) { glUniform3f(get_uniform_location(name), value.x, value.y, value.z); }

		inline void set_uniform_vec4(std::string_view name, const vec4 &value) {
			glUniform4f(get_uniform_location(name), value.x, value.y, value.z, value.w);
		}

		inline void set_uniform_mat3(std::string_view name, const mat3 &value) {
			glUniformMatrix3fv(get_uniform_location(name), 1, 0u, value_ptr(value));
		}

		inline void set_uniform_mat4(std::string_view name, const mat4 &value) {
			glUniformMatrix4fv(get_uniform_location(name), 1, 0u, value_ptr(value));
		}

	private:
		i32 get_uniform_location(std::string_view name);

		static u32 compile(GLenum type, const char *src);
		static bool check_status(u32 id, GLenum type, bool program = true);
		static void print_info_log(u32 id, bool program = true);

	private:
		bool m_Valid = false;
		std::unordered_map<std::string_view, i32> m_UniformLocations;
		u32 m_ID = 0;
	};
}// namespace fif::gfx
