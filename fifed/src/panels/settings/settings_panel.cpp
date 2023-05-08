#include "settings_panel.hpp"
#include "../../camera_controller.hpp"
#include "../../grid.hpp"

namespace fifed {
	void SettingsPanel::on_render() {
		if(ImGui::TreeNode("Grid")) {
			ImGui::Checkbox("Enabled", &Grid::enabled);
			ImGui::SliderFloat("Line tickness", &Grid::lineThickness, 0.0f, 1.0f);
			ImGui::SliderFloat("Cell size", &Grid::cellSize, 0.1f, 100.0f);

			glm::vec4 colorNormalized = get_normalized_color(Grid::lineColor);
			ImGui::ColorEdit4("Line color", glm::value_ptr(colorNormalized));
			Grid::lineColor = get_color_from_normalized(colorNormalized);
			ImGui::SliderFloat("Wrap Value", &Grid::wrapValue, 1.0f, 1000.0f);

			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Camera controller")) {
			ImGui::SliderFloat("Zoom lerp speed", &CameraController::zoomLerpSpeed, 0.1f, 100.0f);
			ImGui::SliderFloat("Min zoom", &CameraController::minZoom, 0.001f, 0.1f);
			ImGui::SliderFloat("Max zoom", &CameraController::maxZoom, 1.0f, 1000.0f);

			ImGui::TreePop();
		}
	}
}// namespace fifed
