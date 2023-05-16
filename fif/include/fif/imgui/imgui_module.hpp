#pragma once

#include "fif/core/module.hpp"

#include "imgui.h"

namespace fif::imgui {
	typedef void (*ImGuiRenderFunc)();

	class ImGuiModule final : public core::Module {
	public:
		FIF_MODULE(ImGuiModule)

		ImGuiModule();
		virtual ~ImGuiModule();

		inline void add_render_func(ImGuiRenderFunc renderFunc) { m_RenderFunctions.push_back(renderFunc); }
		void delete_render_func(ImGuiRenderFunc renderFunc);

		void on_start(core::Application &app) override final;
		void on_render() override final;
		void on_event(core::Event &event) override final;
		bool begin_dockspace() const;

	private:
		void apply_default_theme() const;

	private:
		std::vector<ImGuiRenderFunc> m_RenderFunctions;
	};
}// namespace fif::imgui
