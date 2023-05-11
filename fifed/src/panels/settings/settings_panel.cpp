#include "settings_panel.hpp"
#include "../../camera_controller.hpp"
#include "../../grid.hpp"
#include "editor_module.hpp"
#include "gfx_module.hpp"
#include "ortho_camera.hpp"

namespace fifed {
	void SettingsPanel::on_render() {
		EditorModule *editor = EditorModule::get_instance();
		FrameBuffer &frameBuffer = editor->get_frame_buffer();

		glm::vec4 clearColor = get_normalized_color(frameBuffer.m_Color);
		if(ImGui::ColorEdit4("Clear color", glm::value_ptr(clearColor))) {
			Grid::backgroundColor = frameBuffer.m_Color = get_color_from_normalized(clearColor);
		}

		if(ImGui::TreeNode("Grid")) {
			ImGui::Checkbox("Enabled", &Grid::enabled);
			ImGui::SliderFloat("Line tickness", &Grid::lineThickness, 1.0f, 10.0f);
			ImGui::SliderFloat("Cell size", &Grid::minCellSize, 0.1f, 100.0f);

			glm::vec3 lineColorNormalized = get_normalized_color(Grid::lineColor);
			if(ImGui::ColorEdit3("Line color", glm::value_ptr(lineColorNormalized)))
				Grid::lineColor = get_color_from_normalized(lineColorNormalized);

			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Camera controller")) {
			OrthoCamera &cam = GfxModule::get_instance()->get_renderer2D().get_camera();
			ImGui::SliderFloat("Zoom lerp duration", &CameraController::s_ZoomLerpDuration, 0.0f, 3.0f);
			ImGui::SliderFloat("Min zoom", &CameraController::s_MinZoom, 0.001f, 0.1f);
			ImGui::SliderFloat("Max zoom", &CameraController::s_MaxZoom, 1.0f, 1000.0f);
			ImGui::Text("Current zoom: %f", cam.m_Zoom);

			ImGui::TreePop();
		}
	}
}// namespace fifed
