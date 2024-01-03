#include "fif/core/uuid.hpp"
#include <random>

namespace fif::core {
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Generator(s_RandomDevice());
	static std::uniform_int_distribution<u64> s_Distribution;

	UUID::UUID() : m_Value(s_Distribution(s_Generator)) {
	}

	UUID::UUID(const u64 value) : m_Value(value) {
	}
}// namespace fif::core
