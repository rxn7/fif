#pragma once

#include "fif/core/types.hpp"

namespace fif::core {
	class Timing {
	public:
		static inline f32 get_delta_time() { return m_Dt; }

	private:
		static void init();
		static void update();

	private:
		static f32 m_Dt;
		friend class Application;
	};
}// namespace fif::core
