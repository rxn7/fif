#pragma once

namespace fifed {
	enum IconType : u8 { GITHUB, LOGO, PAUSE, UNPAUSE, STOP };

	struct IconInfo {
		vec2 offset;
		vec2 size;
	};

	class IconManager final {
	public:
		IconManager(const std::string_view path);
		void add_icon(IconType type, const IconInfo &info);
		const IconInfo &get_icon(IconType type) const;
		bool imgui_button(const std::string_view id_str, IconType type, const vec2 &size = {32.0f, 32.0f});

	private:
		std::unordered_map<IconType, IconInfo> m_IconInfos;
		Texture m_Texture;
	};
}// namespace fifed
