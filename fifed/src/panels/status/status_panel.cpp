#include "status_panel.hpp"
#include "application.hpp"

namespace fifed {
	void StatusPanel::on_render() {
		Application *app = Application::get_instance();
		const ApplicationStatus &status = app->get_status();

		bool paused = status.paused;
		if(ImGui::Checkbox("Paused", &paused))
			app->pause(paused);
	}
}// namespace fifed
