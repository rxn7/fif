#pragma once

#include "fif/core/types.hpp"

namespace fif::core {
	class Timing {
	public:
		static inline f32 get_delta_time() {
			return s_DeltaTime;
		}

	private:
		static void init();
		static void update();

	private:
		inline static f32 s_DeltaTime;
		friend class Application;
	};
}// namespace fif::core
