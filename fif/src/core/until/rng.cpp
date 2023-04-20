#include "fif/core/util/rng.hpp"

#include <random>

namespace fif::core {
	std::mt19937 Rng::s_Generator;
	std::random_device Rng::s_Device;

	void Rng::init() {
		s_Generator = std::mt19937(s_Device());
	}
} // namespace fif::core
