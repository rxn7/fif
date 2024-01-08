#pragma once

#include "fif/core/application_status.hpp"
#include "fif/core/ecs/scene.hpp"
#include "fif/core/ecs/system.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/core/performance_stats.hpp"
#include "fif/core/window.hpp"

namespace fif::core {
	struct ApplicationProperties final {
		WindowProperties windowProps;
		bool createDefaultScene = true;
	};

	class Application {
	public:
		Application(const ApplicationProperties &appProperties);
		virtual ~Application();

		void start();

		virtual void set_pause(bool paused) { m_Status.paused = paused; }

		virtual void setup_modules() {}
		virtual void on_event(Event &event);

		inline void add_render_system(RenderSystem system) { m_RenderSystems.push_back(system); }
		inline void add_update_system(UpdateSystem system) { m_UpdateSystems.push_back(system); }
		inline void add_event_system(EventSystem system) { m_EventSystems.push_back(system); }

		inline static Application &get_instance() { return *sp_Instance; }
		inline const PerformanceStats &get_performance_stats() const { return m_PerformanceStats; }
		inline const ApplicationStatus &get_status() const { return m_Status; }
		inline Window &get_window() { return *mp_Window; }
		inline Scene &get_scene() { return *mp_Scene; }

	protected:
		template<class T, class... Args> const T &attach_module(Args &&...args) {
			static_assert(std::is_base_of<Module, T>().value, "T doesn't derive from Module!");
			const std::unique_ptr<Module> &mod = m_Modules.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			mod->mp_Application = this;
			return *(static_cast<T *>(mod.get()));
		}

	private:
		void game_loop();
		void update();
		void render();

	public:
		Invokable<> m_StartHook;
		Invokable<Event &> m_EventHook;
		Invokable<> m_PreUpdateHook;
		Invokable<> m_UpdateHook;
		Invokable<> m_PreRenderHook;
		Invokable<> m_RenderHook;
		Invokable<> m_EndFrameHook;

	protected:
		std::unique_ptr<Window> mp_Window;

	private:
		std::vector<std::unique_ptr<Module>> m_Modules;
		std::unique_ptr<Scene> mp_Scene;

		std::vector<RenderSystem> m_RenderSystems;
		std::vector<UpdateSystem> m_UpdateSystems;
		std::vector<EventSystem> m_EventSystems;

		PerformanceStats m_PerformanceStats;
		ApplicationStatus m_Status;
		static inline Application *sp_Instance;
	};
}// namespace fif::core
