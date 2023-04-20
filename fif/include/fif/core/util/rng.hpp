#pragma once

#include <cinttypes>
#include <random>

namespace fif::core {
	class Rng {
	  public:
		static void init();

		static inline bool getBool() {
			return static_cast<bool>(std::uniform_int_distribution<>(0, 1)(s_Generator));
		}

		static inline float getFloat(float min, float max) {
			return std::uniform_real_distribution<float>(min, max)(s_Generator);
		}

		static inline std::int32_t getI32(std::int32_t min, std::int32_t max) {
			return std::uniform_int_distribution<std::int32_t>(min, max)(s_Generator);
		}

		static inline std::uint32_t getU32(std::uint32_t min, std::uint32_t max) {
			return std::uniform_int_distribution<std::uint32_t>(min, max)(s_Generator);
		}

		static inline std::int16_t getI16(std::int16_t min, std::int16_t max) {
			return std::uniform_int_distribution<std::int16_t>(min, max)(s_Generator);
		}

		static inline std::uint16_t getU16(std::uint16_t min, std::uint16_t max) {
			return std::uniform_int_distribution<std::uint16_t>(min, max)(s_Generator);
		}

		static inline std::int8_t getI8(std::int8_t min, std::int8_t max) {
			return std::uniform_int_distribution<std::int8_t>(min, max)(s_Generator);
		}

		static inline std::uint8_t getU8(std::uint8_t min, std::uint8_t max) {
			return std::uniform_int_distribution<std::uint8_t>(min, max)(s_Generator);
		}

	  private:
		static std::mt19937 s_Generator;
		static std::random_device s_Device;
	}; // namespace Rng
} // namespace fif::core
