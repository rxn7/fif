#include "editor_panel.hpp"

void EditorPanel::render() {
	if(ImGui::Begin(get_title()))
		on_render();

	ImGui::End();
}
