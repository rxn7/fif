#pragma once

#include "../editor_panel.hpp"

class EntitiesPanel final : public EditorPanel {
	const char *get_title() const override;
	void on_render() override;
};
