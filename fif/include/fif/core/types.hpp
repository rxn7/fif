#pragma once

#include <cinttypes>
#include <glm/fwd.hpp>

typedef float f32;
typedef double f64;

typedef std::int8_t i8;
typedef std::uint8_t u8;

typedef std::int16_t i16;
typedef std::uint16_t u16;

typedef std::int32_t i32;
typedef std::uint32_t u32;

typedef std::int64_t i64;
typedef std::uint64_t u64;

namespace fif {
	using glm::i8vec2;
	using glm::u8vec2;
	using glm::i16vec2;
	using glm::u16vec2;
	using glm::i32vec2;
	using glm::u32vec2;
	using glm::i64vec2;
	using glm::u64vec2;

	using glm::i8vec3;
	using glm::u8vec3;
	using glm::i16vec3;
	using glm::u16vec3;
	using glm::i32vec3;
	using glm::u32vec3;
	using glm::i64vec3;
	using glm::u64vec3;

	using glm::i8vec4;
	using glm::u8vec4;
	using glm::i16vec4;
	using glm::u16vec4;
	using glm::i32vec4;
	using glm::u32vec4;
	using glm::i64vec4;
	using glm::u64vec4;

	using glm::vec2;
	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;
}// namespace fif
