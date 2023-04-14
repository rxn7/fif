#pragma once

#include "fif/core/layers/layer.h"
#include "fif/core/performanceStats.h"
#include "fif/core/window.h"

#include <vector>
#include <memory>

namespace fif::core {
	class Application {
	public:
		Application(const WindowProperties &windowProperties);
		virtual ~Application();

		inline static const Application &getInstance() { return *s_Instance; } 
		inline const Window &getWindow() const { return *mp_Window; }
		inline const PerformanceStats &getLastFramePerformanceStats() const { return m_LastFramePerformanceStats; }
		void start();

	private:
		void startFrame();
		void endFrame();

	private:
		static Application *s_Instance;
		PerformanceStats m_LastFramePerformanceStats;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::unique_ptr<Window> mp_Window;
	};
}
