#include "fif/core/ecs/uuid.hpp"

#include <functional>
#include <random>

// TODO: Check for collisions

namespace fif::core {
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Generator(s_RandomDevice());
	static std::uniform_int_distribution<u64> s_Dist;

	UUID::UUID() : m_UUID(s_Dist(s_Generator)) {}

	UUID::UUID(u64 uuid) : m_UUID(uuid) {}
}// namespace fif::core
