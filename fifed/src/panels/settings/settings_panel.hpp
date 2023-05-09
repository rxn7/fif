#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class SettingsPanel final : public EditorPanel {
		PANEL_NAME("Settings")

		void on_render() override;
	};
}// namespace fifed