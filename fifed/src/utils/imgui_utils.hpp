#pragma once

namespace fifed::utils {
	namespace imgui {
		void draw_color_picker(const std::string &title, Color &color);
		void draw_color3_picker(const std::string &title, Color3 &color);
	}// namespace imgui
}// namespace fifed::utils
