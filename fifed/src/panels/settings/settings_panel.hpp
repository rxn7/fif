#pragma once

#include "../editor_panel.hpp"

namespace fifed {
	class SettingsPanel final : public EditorPanel {
		const char *get_title() const override;
		void on_render() override;
	};
}// namespace fifed
