#include "viewport_panel.hpp"

namespace fifed {
	ViewportPanel::ViewportPanel(FrameBuffer &frameBuffer) : m_FrameBuffer(frameBuffer) {}

	void ViewportPanel::on_render() {
		ImGui::BeginChild("FrameBuffer");

		const ImVec2 pos = ImGui::GetWindowPos();
		const ImVec2 size = ImGui::GetWindowSize();

		GfxModule::get_instance()->set_viewport(glm::vec2(size.x, size.y), glm::vec2(pos.x, pos.y));
		m_FrameBuffer.set_size(glm::vec2(size.x, size.y));

		ImGui::Image(reinterpret_cast<ImTextureID>(m_FrameBuffer.getTexture()->get_id()), size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		m_Hovered = ImGui::IsItemHovered();

		ImGui::EndChild();
	}
}// namespace fifed
