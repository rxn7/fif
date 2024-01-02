#include "icon_manager.hpp"

namespace fifed {
	IconManager::IconManager(const std::filesystem::path &path) : m_Texture(true, path, GL_NEAREST) {
	}

	void IconManager::add_icon(IconType type, const IconInfo &info) {
		if(m_IconInfos.find(type) != m_IconInfos.end()) {
			Logger::error("Tried to add icon that exists: %i!", type);
			return;
		}

		m_IconInfos.emplace(type, info);
	}

	const IconInfo &IconManager::get_icon(IconType type) const {
		const auto &it = m_IconInfos.find(type);

		if(it == m_IconInfos.end()) {
			Logger::error("This icon doesn't exist: %i!", type);
			return m_IconInfos.begin()->second;
		}

		return it->second;
	}

	bool IconManager::imgui_button(const std::string_view idStr, IconType type, const vec2 &size) {
		const IconInfo &info = get_icon(type);
		const vec2 uv0 = vec2(info.offset.x / m_Texture.get_width(), 1.0f - info.offset.y / m_Texture.get_height());
		const vec2 uv1 = vec2((info.offset.x + info.size.x) / m_Texture.get_width(), 1.0f - (info.offset.y + info.size.y) / m_Texture.get_height());
		return ImGui::ImageButton(idStr.data(), reinterpret_cast<ImTextureID>(m_Texture.get_id()), ImVec2(size.x, size.y), ImVec2(uv0.x, uv0.y), ImVec2(uv1.x, uv1.y));
	}
}// namespace fifed
