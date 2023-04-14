#pragma once

#include "entt/signal/fwd.hpp"
#include "fif/core/events/updateEvent.h"
#include "fif/core/layers/layer.h"
#include "fif/core/module.h"
#include "fif/core/performanceStats.h"
#include "fif/core/window.h"
#include "entt/signal/dispatcher.hpp"

#include <vector>
#include <memory>

namespace fif::core {
	class Application {
	public:
		Application(const WindowProperties &windowProperties);
		virtual ~Application();

		void start();
		void registerModule(const Module *mod);

		inline static Application &getInstance() { 
			return *s_Instance;
		} 

		inline const Window &getWindow() const {
			return *mp_Window;
		}

		inline const PerformanceStats &getLastFramePerformanceStats() const { 
			return m_LastFramePerformanceStats;
		}
		
	private:
		void startFrame(float dt);
		void endFrame();

	protected:
		std::vector<const Module*> m_Modules;
		std::vector<std::unique_ptr<Layer>> m_Layers;
		std::unique_ptr<Window> mp_Window;

	private:
		PerformanceStats m_LastFramePerformanceStats;
		static Application *s_Instance;
	};
}
