#include "editor_panel.hpp"

namespace fifed {
	void EditorPanel::render() {
		if(ImGui::Begin(get_title()))
			on_render();

		ImGui::End();
	}
}// namespace fifed
