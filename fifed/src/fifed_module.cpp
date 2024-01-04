#include "fifed_module.hpp"
#include "camera_controller.hpp"
#include "editor.hpp"
#include "grid.hpp"
#include "project_manager.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/event/key_event.hpp"
#include "fif/core/serialization/project_serializer.hpp"
#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fifed {
	FIF_MODULE_INSTANCE_IMPL(FifedModule);

	FifedModule::FifedModule() : m_IconManager("assets/textures/icons.png"), m_ImGuiRenderCallback(std::bind(&FifedModule::render_imgui, this)) {
		FIF_MODULE_INIT_INSTANCE();
	}

	FifedModule::~FifedModule() {
		ImGuiModule::get_instance()->get_render_hook().unhook(m_ImGuiRenderCallback);
	}

	void FifedModule::on_start() {
		mp_Application->set_pause(true);

		static constexpr ImWchar ranges[] = {0x0020, 0x017f, 0};
		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 18, nullptr, ranges);
		io.IniFilename = "layout.ini";

		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		ImGuiModule::get_instance()->get_render_hook().hook(m_ImGuiRenderCallback);
		mp_Stage = std::make_unique<ProjectManager>(*this);
	}

	void FifedModule::load_default_layout() {
		ImGui::LoadIniSettingsFromDisk("default_layout.ini");
	}

	void FifedModule::render_imgui() {
		mp_Stage->render_imgui();
	}

	void FifedModule::on_update() {
		mp_Stage->update();
	}

	void FifedModule::pre_render() {
		mp_Stage->pre_render();
	}

	void FifedModule::on_render() {
		mp_Stage->render();
	}

	void FifedModule::on_project_open() {
		mp_Stage = std::make_unique<Editor>(*this);
	}

	void FifedModule::on_event(Event &event) {
		mp_Stage->on_event(event);
	}
}// namespace fifed
