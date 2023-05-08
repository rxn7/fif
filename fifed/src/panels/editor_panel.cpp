#include "editor_panel.hpp"

namespace fifed {
	void EditorPanel::render() {
		if(ImGui::Begin(get_title().data()))
			on_render();

		ImGui::End();
	}
}// namespace fifed
