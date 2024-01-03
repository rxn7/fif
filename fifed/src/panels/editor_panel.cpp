#include "editor_panel.hpp"

namespace fifed {
	EditorPanel::EditorPanel(Editor &editor) : m_Editor(editor) {
	}

	void EditorPanel::render() {
		if(ImGui::Begin(get_title().data()))
			on_render();

		ImGui::End();
	}
}// namespace fifed
