#include "viewport_panel.hpp"
#include "application.hpp"
#include "editor.hpp"
#include "fifed_module.hpp"
#include "imgui.h"

namespace fifed {
	ViewportPanel::ViewportPanel(Editor &editor, FrameBuffer &frameBuffer) : EditorPanel(editor), m_FrameBuffer(frameBuffer) {}

	void ViewportPanel::on_render() {
		Application *app = FifedModule::get_instance().get_application();
		const bool isPlayMode = m_Editor.is_play_mode();
		const bool isPaused = app->get_status().paused;

		{
			static const char *items[]{"Translate", "Scale"};
			int currentItem = static_cast<int>(m_Editor.get_gizmo().m_Mode);
			ImGui::Combo("Gizmo Mode", &currentItem, items, IM_ARRAYSIZE(items));
			m_Editor.get_gizmo().m_Mode = static_cast<GizmoMode>(currentItem);
		}
		{
			static const char *items[]{"Global", "Local"};
			int currentItem = static_cast<int>(m_Editor.get_gizmo().m_Space);
			ImGui::Combo("Gizmo Space", &currentItem, items, IM_ARRAYSIZE(items));
			m_Editor.get_gizmo().m_Space = static_cast<GizmoSpace>(currentItem);
		}

		const f32 windowWidth = ImGui::GetWindowSize().x;
		ImGui::SetCursorPosX((windowWidth - 32.0f) * 0.5f);

		if(isPlayMode) {
			if(FifedModule::get_instance().get_icon_manager().imgui_button("Pause", isPaused ? IconType::UNPAUSE : IconType::PAUSE))
				app->set_pause(!isPaused);

			ImGui::SameLine();

			if(ImGui::SmallButton("Stop"))
				m_Editor.set_play_mode(false);
		} else {
			if(ImGui::SmallButton("Start"))
				m_Editor.set_play_mode(true);
		}

		ImGui::BeginChild("FrameBuffer");

		static vec2 lastPos = {-1, -1};
		static vec2 lastSize = {-1, -1};
		const vec2 pos = ImGui::GetWindowPos();
		const vec2 size = ImGui::GetWindowSize();

		if(m_Resize || lastPos != pos || lastSize != size) {
			m_Resize = false;

			m_FrameBuffer.set_size(size);
			GfxModule::get_instance().set_viewport(size, pos);
		}

		lastSize = size;
		lastPos = pos;

		ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer.get_texture()->get_id()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		m_Hovered = ImGui::IsItemHovered();

		ImGui::EndChild();
	}
}// namespace fifed
