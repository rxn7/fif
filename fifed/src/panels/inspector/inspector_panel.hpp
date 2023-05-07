#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class InspectorPanel final : public EditorPanel {
	public:
		const char *get_title() const override;
		void on_render() override;

		static void draw_color_selector(Color &color);

	public:
		EntityID m_SelectedEntity = entt::null;
		std::array<char, 100> m_TagBuffer;
	};
}// namespace fifed
