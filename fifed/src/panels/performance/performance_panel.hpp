#pragma once

#include "../editor_panel.hpp"

class PerformancePanel final : public EditorPanel {
	const char *get_title() const override;
	void on_render() override;
};
