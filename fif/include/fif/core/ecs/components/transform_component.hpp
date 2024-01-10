#pragma once

namespace fif::core {
	struct TransformComponent final {
		vec2 position = {0.0f, 0.0f};
		vec2 scale = {1.0f, 1.0f};
		f32 angleRadians = 0.0f;

		inline mat3 get_rotation_matrix() const { return glm::rotate(mat4(1.0f), -angleRadians, vec3(0.0f, 0.0f, 1.0f)); }

		// NOTE: If you use get_up_dir or get_right_dir multiple times in a frame you should cache get_rotation_matrix() and pass it to get_up_dir_ex(const mat4 &)!
		inline vec2 get_up_dir() const { return get_up_dir_ex(get_rotation_matrix()); }

		// NOTE: If you use get_up_dir or get_right_dir multiple times in a frame you should cache get_rotation_matrix() and pass it to get_right_dir_ex(const mat4 &)!
		inline vec2 get_right_dir() const { return get_right_dir_ex(get_rotation_matrix()); }

		inline vec2 get_up_dir_ex(const mat3 &rotationMatrix) const { return rotationMatrix * vec3(0.0f, 1.0f, 0.0f); }
		inline vec2 get_right_dir_ex(const mat3 &rotationMatrix) const { return rotationMatrix * vec3(1.0f, 0.0f, 0.0f); }
	};
}// namespace fif::core
