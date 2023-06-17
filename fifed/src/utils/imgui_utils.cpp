#include "imgui_utils.hpp"

namespace fifed::utils {
	void imgui::draw_color_picker(std::string_view title, Color &color) {
		fif::vec4 normalizedColor = normalize_color(color);
		if(ImGui::ColorEdit4(title.data(), value_ptr(normalizedColor)))
			color = denormalize_color(normalizedColor);
	}

	void imgui::draw_color3_picker(std::string_view title, Color3 &color) {
		fif::vec3 normalizedColor = normalize_color3(color);
		if(ImGui::ColorEdit3(title.data(), value_ptr(normalizedColor)))
			color = denormalize_color3(normalizedColor);
	}
}// namespace fifed::utils
