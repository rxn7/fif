#pragma once

#include "glm/fwd.hpp"
#include <cinttypes>

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
	typedef glm::i8vec2 i8vec2;
	typedef glm::u8vec2 u8vec2;
	typedef glm::i16vec2 i16vec2;
	typedef glm::u16vec2 u16vec2;
	typedef glm::i32vec2 i32vec2;
	typedef glm::u32vec2 u32vec2;
	typedef glm::i64vec2 i64vec2;
	typedef glm::u64vec2 u64vec2;

	typedef glm::i8vec3 i8vec3;
	typedef glm::u8vec3 u8vec3;
	typedef glm::i16vec3 i16vec3;
	typedef glm::u16vec3 u16vec3;
	typedef glm::i32vec3 i32vec3;
	typedef glm::u32vec3 u32vec3;
	typedef glm::i64vec3 i64vec3;
	typedef glm::u64vec3 u64vec3;

	typedef glm::i8vec4 i8vec4;
	typedef glm::u8vec4 u8vec4;
	typedef glm::i16vec4 i16vec4;
	typedef glm::u16vec4 u16vec4;
	typedef glm::i32vec4 i32vec4;
	typedef glm::u32vec4 u32vec4;
	typedef glm::i64vec4 i64vec4;
	typedef glm::u64vec4 u64vec4;

	typedef glm::vec2 vec2;
	typedef glm::vec3 vec3;
	typedef glm::vec4 vec4;
	typedef glm::mat3 mat3;
	typedef glm::mat4 mat4;
}// namespace fif
