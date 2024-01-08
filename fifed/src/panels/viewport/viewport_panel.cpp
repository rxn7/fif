#include "viewport_panel.hpp"
#include "application.hpp"
#include "editor.hpp"
#include "fifed_module.hpp"

namespace fifed {
	ViewportPanel::ViewportPanel(Editor &editor, FrameBuffer &frameBuffer) : EditorPanel(editor), m_FrameBuffer(frameBuffer) {}

	void ViewportPanel::on_render() {
		ImGui::SameLine();

		Application *app = FifedModule::get_instance().get_application();
		const bool isPlayMode = m_Editor.is_play_mode();
		const bool isPaused = app->get_status().paused;
		const f32 windowWidth = ImGui::GetWindowSize().x;

		ImGui::SetCursorPosX((windowWidth - 32.0f) * 0.5f);

		if(isPlayMode) {
			if(FifedModule::get_instance().get_icon_manager().imgui_button("Pause", isPaused ? IconType::UNPAUSE : IconType::PAUSE))
				app->set_pause(!isPaused);

			ImGui::SameLine();

			if(ImGui::Button("Stop"))
				m_Editor.set_play_mode(false);
		} else {
			if(ImGui::Button("Start"))
				m_Editor.set_play_mode(true);
		}

		ImGui::BeginChild("FrameBuffer");

		const vec2 pos = ImGui::GetWindowPos();
		const vec2 size = ImGui::GetWindowSize();

		if(m_LastSize != size || m_Resize) {
			m_Resize = false;

			m_FrameBuffer.set_size(size);
			GfxModule::get_instance().set_viewport(size, pos);
		}
		m_LastSize = size;

		ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer.get_texture().get_id()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		m_Hovered = ImGui::IsItemHovered();

		ImGui::EndChild();
	}
}// namespace fifed
