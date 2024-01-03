#pragma once

#include "./stage.hpp"
#include <memory>

namespace fifed {
	class FifedModule final : public Module {
	public:
		FIF_MODULE(FifedModule)

		FifedModule();
		virtual ~FifedModule();

		void load_default_layout();
		void on_project_open();

	protected:
		void on_start() override;
		void on_render() override;
		void pre_render() override;
		void on_update() override;
		void on_event(Event &event) override;

	private:
		void render_imgui();

	private:
		Callback<> m_ImGuiRenderCallback;
		std::unique_ptr<Stage> mp_Stage = nullptr;

		friend Stage;
	};
}// namespace fifed
