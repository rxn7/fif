#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class EntitiesPanel final : public EditorPanel {
	public:
		const char *get_title() const override;
		void on_render() override;

	private:
		void draw_entity(EntityID ent, Scene &scene);
		void draw_components(EntityID ent, Scene &scene);
	};
}// namespace fifed
