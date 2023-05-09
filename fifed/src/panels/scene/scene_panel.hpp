#pragma once

#include "../editor_panel.hpp"
#include "../inspector/inspector_panel.hpp"

namespace fifed {
	class ScenePanel final : public EditorPanel {
	public:
		PANEL_NAME("Scene")

		ScenePanel(InspectorPanel &inspector);

		void on_render() override;

	private:
		void draw_entity(EntityID ent, Scene &scene);

	private:
		InspectorPanel &m_Inspector;
	};
}// namespace fifed
