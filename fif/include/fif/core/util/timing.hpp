#pragma once

namespace fif::core {
	class Timing {
	public:
		static inline float get_delta_time() { return m_Dt; }

	private:
		friend class Application;
		static void update();

	private:
		static float m_Dt;
	};
}// namespace fif::core
