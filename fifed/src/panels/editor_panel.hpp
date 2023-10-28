#pragma once

#include "event/event.hpp"
#include "icon_manager.hpp"
#include "imgui.h"

namespace fifed {
	class EditorPanel {
	public:
		virtual std::string_view get_title() const = 0;
		virtual void on_update([[maybe_unused]] float dt) {}
		virtual void on_event([[maybe_unused]] Event &event) {}
		void render();

	private:
		virtual void on_render() {}

	public:
		IconManager *mp_IconManager;
	};
}// namespace fifed

#define PANEL_NAME(x)                                                                                                                                \
	std::string_view get_title() const override { return x; }
