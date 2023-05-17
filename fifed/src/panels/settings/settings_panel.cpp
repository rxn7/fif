#include "settings_panel.hpp"
#include "../../camera_controller.hpp"
#include "../../grid.hpp"
#include "editor_module.hpp"

#include "fif/gfx/gfx_module.hpp"
#include "fif/gfx/ortho_camera.hpp"

namespace fifed {
	SettingsPanel::SettingsPanel(Grid &grid, FrameBuffer &frameBuffer) : m_Grid(grid), m_FrameBuffer(frameBuffer) {}

	void SettingsPanel::on_render() {
		if(ImGui::TreeNode("Viewport")) {
			utils::imgui::draw_color3_picker("Viewport clear color", m_FrameBuffer.m_Color);
			ImGui::TreePop();
		}

		if(ImGui::TreeNode("Grid")) {
			ImGui::Checkbox("Enabled", &m_Grid.m_Enabled);
			ImGui::SliderFloat("Line tickness", &m_Grid.m_LineThickness, 1.0f, 10.0f);
			ImGui::SliderFloat("Cell size", &m_Grid.m_MinCellSize, 0.1f, 100.0f);

			utils::imgui::draw_color3_picker("Line color", m_Grid.m_LineColor);

			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Camera controller")) {
			OrthoCamera &cam = GfxModule::get_instance()->get_renderer2D().get_camera();
			ImGui::SliderFloat("Zoom lerp duration", &CameraController::s_ZoomLerpDuration, 0.0f, 3.0f);
			ImGui::SliderFloat("Min zoom", &CameraController::s_MinZoom, 0.005f, 0.1f);
			ImGui::SliderFloat("Max zoom", &CameraController::s_MaxZoom, 1.0f, 1000.0f);
			ImGui::Text("Current zoom: %f", cam.m_Zoom);

			ImGui::TreePop();
		}
	}
}// namespace fifed
