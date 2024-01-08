#pragma once

#include <fif/core/event/event.hpp>

namespace fifed {
	class Editor;

	class EditorPanel {
	public:
		virtual inline constexpr std::string_view get_title() const = 0;
		virtual void on_update([[maybe_unused]] f32 dt) {}
		virtual void on_event([[maybe_unused]] Event &event) {}
		void render();

	protected:
		EditorPanel(Editor &editor);

	private:
		virtual void on_render() {}

	protected:
		Editor &m_Editor;
	};
}// namespace fifed

#define PANEL_NAME(x)                                                                                                                                                                                  \
	inline constexpr std::string_view get_title() const override { return x; }
