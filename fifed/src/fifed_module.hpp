#pragma once

#include "./stage.hpp"
#include "icon_manager.hpp"
#include <memory>

namespace fifed {
	class FifedModule final : public Module {
	public:
		FIF_MODULE(FifedModule)

		FifedModule();
		virtual ~FifedModule();

		void load_default_layout();
		void on_project_open();

		inline IconManager &get_icon_manager() {
			return m_IconManager;
		}

	protected:
		void on_start() override;
		void on_render() override;
		void pre_render() override;
		void on_update() override;
		void on_event(Event &event) override;

	private:
		void render_imgui();

	public:
		std::unique_ptr<Stage> mp_Stage = nullptr;

	private:
		IconManager m_IconManager;
		Callback<> m_ImGuiRenderCallback;
		friend Stage;
	};
}// namespace fifed
