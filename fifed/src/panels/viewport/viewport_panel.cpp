#include "viewport_panel.hpp"
#include "application.hpp"

namespace fifed {
	ViewportPanel::ViewportPanel(FrameBuffer &frameBuffer) : m_FrameBuffer(frameBuffer) {}

	void ViewportPanel::on_render() {
		Application *app = Application::get_instance();
		const ApplicationStatus &status = app->get_status();

		ImGui::SameLine();
		const f32 windowWidth = ImGui::GetWindowSize().x;

		ImGui::SetCursorPosX((windowWidth - 32.0f) * 0.5f);
		if(mp_IconManager->imgui_button("Pause", status.paused ? IconType::UNPAUSE : IconType::PAUSE))
			app->set_pause(!status.paused);

		ImGui::BeginChild("FrameBuffer");

		const ImVec2 pos = ImGui::GetWindowPos();
		const ImVec2 size = ImGui::GetWindowSize();

		m_FrameBuffer.set_size(glm::vec2(size.x, size.y));
		GfxModule::get_instance()->set_viewport(glm::vec2(size.x, size.y), glm::vec2(pos.x, pos.y));

		ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer.get_texture().get_id()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		m_Hovered = ImGui::IsItemHovered();

		ImGui::EndChild();
	}
}// namespace fifed
