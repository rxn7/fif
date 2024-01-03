#pragma once

#include "../editor_panel.hpp"
#include "../inspector/inspector_panel.hpp"
#include "entity_template/entity_template.hpp"

namespace fifed {
	class ScenePanel final : public EditorPanel {
	public:
		ScenePanel(Editor &editor, InspectorPanel &inspector);
		PANEL_NAME("Scene")

		void on_render() override;

	private:
		void draw_entity(EntityID ent, const char *name, Scene &scene);

	private:
		InspectorPanel &m_Inspector;
		std::vector<std::unique_ptr<EntityTemplate>> m_Templates;
	};
}// namespace fifed
