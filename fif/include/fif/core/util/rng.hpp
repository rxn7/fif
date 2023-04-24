#pragma once

#include "fif/core/types.hpp"
#include <random>

namespace fif::core {
	class Rng {
	public:
		static void init();

		static inline bool get_bool() { return static_cast<bool>(std::uniform_int_distribution<>(0, 1)(s_Generator)); }

		static inline f32 get_f32(f32 min, f32 max) { return std::uniform_real_distribution<f32>(min, max)(s_Generator); }

		static inline i32 get_i32(i32 min, i32 max) { return std::uniform_int_distribution<i32>(min, max)(s_Generator); }

		static inline u32 get_u32(u32 min, u32 max) { return std::uniform_int_distribution<u32>(min, max)(s_Generator); }

		static inline i16 get_i16(i16 min, i16 max) { return std::uniform_int_distribution<i16>(min, max)(s_Generator); }

		static inline u16 get_u16(u16 min, u16 max) { return std::uniform_int_distribution<u16>(min, max)(s_Generator); }

		static inline i8 get_i8(i8 min, i8 max) { return std::uniform_int_distribution<i8>(min, max)(s_Generator); }

		static inline u8 get_u8(i8 min, i8 max) { return std::uniform_int_distribution<u8>(min, max)(s_Generator); }

	private:
		static std::mt19937 s_Generator;
		static std::random_device s_Device;
	};// namespace Rng
}// namespace fif::core
