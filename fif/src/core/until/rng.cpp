#include "fif/core/util/rng.hpp"

namespace fif::core {
	void Rng::init() {
		s_Generator = std::mt19937(s_Device());
	}
}// namespace fif::core
