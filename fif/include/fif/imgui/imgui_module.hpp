#pragma once

#include "fif/core/module.hpp"

#include <functional>

namespace fif::imgui {
	class ImGuiModule final : public core::Module {
	public:
		FIF_MODULE_NAME(ImGui)
		FIF_MODULE_INSTANCE_FUNC_DECL(ImGuiModule);

		ImGuiModule();
		virtual ~ImGuiModule();

		inline void add_render_func(const std::function<void()> &renderFunc) { m_RenderFunctions.push_back(renderFunc); }

		void on_start(core::Application &app) override final;
		void on_render() override final;
		void on_event(core::Event &event) override final;
		bool begin_dockspace() const;

	private:
		void apply_default_theme() const;

	private:
		std::vector<std::function<void()>> m_RenderFunctions;
	};
}// namespace fif::imgui
