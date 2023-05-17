#pragma once

namespace fifed {
	namespace utils {
		namespace imgui {
			void draw_color_picker(std::string_view title, Color &color);
			void draw_color3_picker(std::string_view title, Color3 &color);
		}// namespace imgui
	}	 // namespace utils
}// namespace fifed