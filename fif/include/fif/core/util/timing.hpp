#pragma once

namespace fif::core {
	class Time {
	  public:
		static inline float getDeltaTime() { return m_Dt; }

	  private:
		friend class Application;
		static void update();

	  private:
		static float m_Dt;
	};
} // namespace fif::core
