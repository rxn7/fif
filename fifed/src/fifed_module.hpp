#pragma once

#include "icon_manager.hpp"
#include "stage.hpp"

#include <fif/core/resource/resource_manager.hpp>

#include <memory>

namespace fifed {
	class FifedModule final : public Module {
	public:
		FIF_MODULE(FifedModule)

		FifedModule();
		~FifedModule();

		void load_default_layout();
		void on_project_open();

		inline const IconManager &get_icon_manager() const { return m_IconManager; }

		void open_project_manager();
		void open_editor();

	private:
		void on_start();
		void on_update();
		void pre_render();
		void on_render();
		void on_render_imgui();
		void on_event(Event &event);

	private:
		Callback<> m_StartCallback;
		Callback<> m_UpdateCallback;
		Callback<> m_PreRenderCallback;
		Callback<> m_RenderCallback;
		Callback<Event &> m_EventCallback;
		Callback<> m_ImGuiRenderCallback;

		std::unique_ptr<Stage> mp_Stage = nullptr;
		IconManager m_IconManager;
		friend Stage;
	};
}// namespace fifed
