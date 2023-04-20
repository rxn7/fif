#pragma once

#include "fif/core/ecs/entity.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/core/performanceStats.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/clock.hpp"
#include "fif/core/window.hpp"

#include <algorithm>
#include <chrono>
#include <memory>
#include <set>
#include <vector>

namespace fif::core {
	class Application {
	  public:
		Application(const WindowProperties &windowProperties);
		virtual ~Application();

		void start();

		virtual void onEvent(Event &event);

		inline static Application &getInstance() { return *s_Instance; }

		inline const Window &getWindow() const { return *mp_Window; }

		inline const PerformanceStats &getPerformanceStats() const { return m_PerformanceStats; }

		inline const std::vector<Entity> &getEntities() const { return m_Entities; }

		inline Entity *createEntity() { return &m_Entities.emplace_back(); }

	  protected:
		template <class T, class... Args> void attachModule(Args &&...args) {
			static_assert(std::is_base_of<Module, T>().value, "T doesn't derive from Module!");

			FIF_PROFILE_FUNC();

			std::unique_ptr<Module> &mod = m_Modules.emplace_back(std::make_unique<T>(args...));
			FIF_LOG("Module " << mod->getName() << " attached");
		}

	  private:
		void gameLoop();
		void update();
		void render();

	  protected:
		std::unique_ptr<Window> mp_Window;

	  private:
		std::vector<Entity> m_Entities;
		std::vector<std::unique_ptr<Module>> m_Modules;

		PerformanceStats m_PerformanceStats;
		static Application *s_Instance;
	};
} // namespace fif::core
