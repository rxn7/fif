#pragma once

namespace fifed {
	enum IconType : u8 { GITHUB, LOGO, PAUSE, UNPAUSE, DELETE };

	struct IconInfo final {
		vec2 offset;
		vec2 size;
	};

	class IconManager final {
	public:
		IconManager(const std::filesystem::path &path);

		const IconInfo &get_icon(IconType type) const;
		bool imgui_button(const std::string_view idStr, IconType type, const vec2 &size = {32.0f, 32.0f}) const;

	private:
		std::unordered_map<IconType, IconInfo> m_IconInfos;
		Texture m_Texture;
	};
}// namespace fifed
