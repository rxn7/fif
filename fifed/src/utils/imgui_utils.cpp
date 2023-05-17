#include "imgui_utils.hpp"

namespace fifed::utils {
	void imgui::draw_color_picker(std::string_view title, Color &color) {
		glm::vec4 normalizedColor = normalize_color(color);
		if(ImGui::ColorEdit4(title.data(), glm::value_ptr(normalizedColor)))
			color = denormalize_color(normalizedColor);
	}

	void imgui::draw_color3_picker(std::string_view title, Color3 &color) {
		glm::vec3 normalizedColor = normalize_color3(color);
		if(ImGui::ColorEdit3(title.data(), glm::value_ptr(normalizedColor)))
			color = denormalize_color3(normalizedColor);
	}
}// namespace fifed::utils