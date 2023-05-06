#pragma once

#include "common.hpp"
#include "panels/editor_panel.hpp"
#include "panels/viewport/viewport_panel.hpp"

#include "fif/gfx/frame_buffer.hpp"
#include "fif/gfx/texture.hpp"

namespace fifed {
	class EditorModule final : public Module {
	public:
		FIF_MODULE(EditorModule)

		EditorModule();
		virtual ~EditorModule();

		void on_start(Application &app) override;
		void on_render() override;
		void on_update() override;
		void on_event(Event &event) override;

		template<class T, class... Args> T *add_panel(Args &&...args) {
			std::unique_ptr<EditorPanel> &panel = m_Panels.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			return reinterpret_cast<T *>(panel.get());
		}

	private:
		void on_render_im_gui();

	private:
		ViewportPanel *mp_ViewportPanel;
		std::vector<std::unique_ptr<EditorPanel>> m_Panels;
		std::unique_ptr<FrameBuffer> mp_FrameBuffer;
		std::unique_ptr<Texture> mp_LogoTexture;
	};
}// namespace fifed
