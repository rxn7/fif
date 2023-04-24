#pragma once

#include "fif/core/ecs/entity.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/core/performance_stats.hpp"
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
		Application(const WindowProperties &windowProperties, bool createDefaultScene = true);
		virtual ~Application();

		void start();

		virtual void on_event(Event &event);

		inline static Application &get_instance() { return *s_Instance; }

		inline const Window &get_window() const { return *mp_Window; }

		inline const PerformanceStats &get_performance_stats() const { return m_PerformanceStats; }

	protected:
		template<class T, class... Args> void attach_module(Args &&...args) {
			static_assert(std::is_base_of<Module, T>().value, "T doesn't derive from Module!");

			FIF_PROFILE_FUNC();

			std::unique_ptr<Module> &mod = m_Modules.emplace_back(std::make_unique<T>(args...));
			FIF_LOG("Module " << mod->get_name() << " attached");
		}

	public:
		std::shared_ptr<Scene> mp_Scene;

	private:
		void game_loop();
		void update();
		void render();

	protected:
		std::unique_ptr<Window> mp_Window;

	private:
		std::vector<std::unique_ptr<Module>> m_Modules;

		PerformanceStats m_PerformanceStats;
		static Application *s_Instance;
	};
}// namespace fif::core
