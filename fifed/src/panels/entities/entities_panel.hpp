#pragma once

#include "../editor_panel.hpp"
#include "../inspector/inspector_panel.hpp"

namespace fifed {
	class EntitiesPanel final : public EditorPanel {
	public:
		EntitiesPanel(InspectorPanel &inspector);
		const char *get_title() const override;
		void on_render() override;

	private:
		void draw_entity(EntityID ent, Scene &scene);

	private:
		InspectorPanel &m_Inspector;
	};
}// namespace fifed
