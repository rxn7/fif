#pragma once

#include "fif/core/module.h"
#include <functional>

namespace fif::imgui {
	class ImGuiModule final : public core::Module {
	public:
		ImGuiModule();
		virtual ~ImGuiModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(ImGuiModule);

		constexpr std::string_view getName() const override { return "ImGui"; }

		inline void addRenderFunc(const std::function<void()> &renderFunc) {
			m_RenderFunctions.push_back(renderFunc);
		}

		void onStart(core::Application &app) override;
		void onUpdate(float dt) override;
		void onRender() override;

	private:
		std::vector<std::function<void()>> m_RenderFunctions;
	};
}
