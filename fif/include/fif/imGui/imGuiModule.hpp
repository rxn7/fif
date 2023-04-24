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

		inline void addRenderFunc(const std::function<void()> &renderFunc) { m_RenderFunctions.push_back(renderFunc); }

		void onStart(core::Application &app) override final;
		void onRender() override final;
		void onEvent(core::Event &event) override final;

	private:
		void applyDefaultTheme() const;

	private:
		std::vector<std::function<void()>> m_RenderFunctions;
	};
}// namespace fif::imgui
