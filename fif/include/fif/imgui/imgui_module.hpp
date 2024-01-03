#pragma once

#include "fif/core/module.hpp"

#include "imgui.h"
#include "invokable.hpp"

namespace fif::imgui {
	typedef std::function<void(void)> ImGuiRenderFunc;

	class ImGuiModule final : public core::Module {
	public:
		FIF_MODULE(ImGuiModule)

		ImGuiModule();
		virtual ~ImGuiModule();

		bool begin_dockspace() const;
		inline Invokable<> &get_render_hook() {
			return m_RenderHook;
		}

	protected:
		void on_start() override;
		void on_render() override;
		void on_event(core::Event &event) override;

	private:
		void apply_default_theme() const;

	private:
		Invokable<> m_RenderHook;
	};
}// namespace fif::imgui
