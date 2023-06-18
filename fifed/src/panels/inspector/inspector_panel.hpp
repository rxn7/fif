#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class InspectorPanel final : public EditorPanel {
	public:
		InspectorPanel(Scene &scene);
		PANEL_NAME("Inspector")

		void on_render() override;

		static void draw_color_selector(Color &color);

	public:
		Entity m_SelectedEntity;
		std::array<char, 100> m_TagBuffer;
	};
}// namespace fifed
