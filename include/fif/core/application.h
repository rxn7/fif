#pragma once

#include "fif/core/clock.h"
#include "fif/core/ecs/entity.h"
#include "fif/core/module.h"
#include "fif/core/performanceStats.h"
#include "fif/core/profiler.h"
#include "fif/core/window.h"
#include "fif/core/event/event.h"

#include <chrono>
#include <vector>
#include <set>
#include <memory>

namespace fif::core {
	class Application {
	public:
		Application(const WindowProperties &windowProperties);
		virtual ~Application();

		void start();

		virtual void onEvent(Event &event);

		inline static Application &getInstance() { return *s_Instance; } 
		inline const Window &getWindow() const { return *mp_Window; }
		inline const PerformanceStats &getLastFramePerformanceStats() const { return m_LastFramePerformanceStats; }
		inline const std::vector<Entity> &getEntities() const { return m_Entities; }
		inline Entity *createEntity() { return &m_Entities.emplace_back(); }

	protected:
		template<class T, class ...Args>
		void attachModule(Args &&...args) {
			static_assert(std::is_base_of<Module, T>().value, "T doesn't derive from Module!");

			FIF_PROFILE_FUNC();

			std::unique_ptr<Module> &mod = m_Modules.emplace_back(std::make_unique<T>(args...));
			FIF_LOG("Module " << mod->getName() << " attached");
		}
		
	private:
		void gameLoop();
		void update(float dt);
		void render();

	protected:
		std::unique_ptr<Window> mp_Window;

	private:
		std::vector<Entity> m_Entities;
		std::vector<std::unique_ptr<Module>> m_Modules;

		Clock::time_point m_LastFrameTime;
		PerformanceStats m_LastFramePerformanceStats;
		static Application *s_Instance;
	};
}
