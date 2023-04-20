#pragma once

#include <chrono>
#include <type_traits>

namespace fif::core {
	typedef std::conditional<std::chrono::high_resolution_clock::is_steady,
							 std::chrono::high_resolution_clock, std::chrono::steady_clock>::type
		Clock;
	static_assert(Clock::is_steady, "The clock is not steady!");

} // namespace fif::core
