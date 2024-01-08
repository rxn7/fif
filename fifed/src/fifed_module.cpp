#include "fifed_module.hpp"
#include "camera_controller.hpp"
#include "editor.hpp"
#include "grid.hpp"
#include "project_manager.hpp"
#include "stage.hpp"

#include "fif/core/ecs/components/transform_component.hpp"
#include "fif/core/event/key_event.hpp"
#include "fif/core/serialization/project_serializer.hpp"
#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/gfx/gfx_module.hpp"

namespace fifed {
	FifedModule::FifedModule() :
		m_StartCallback(std::bind(&FifedModule::on_start, this)), m_UpdateCallback(std::bind(&FifedModule::on_update, this)), m_PreRenderCallback(std::bind(&FifedModule::pre_render, this)), m_RenderCallback(std::bind(&FifedModule::on_render, this)), m_EventCallback(std::bind(&FifedModule::on_event, this, std::placeholders::_1)), m_ImGuiRenderCallback(std::bind(&FifedModule::on_render_imgui, this)), m_IconManager("assets/textures/icons.png") {
		FIF_MODULE_INIT();

		core::Application &app = core::Application::get_instance();
		app.m_StartHook.hook(m_StartCallback);
		app.m_UpdateHook.hook(m_UpdateCallback);
		app.m_RenderHook.hook(m_RenderCallback);
		app.m_PreRenderHook.hook(m_PreRenderCallback);
		app.m_EventHook.hook(m_EventCallback);
	}

	FifedModule::~FifedModule() {
		mp_Application->m_StartHook.unhook(m_StartCallback);
		mp_Application->m_UpdateHook.unhook(m_UpdateCallback);
		mp_Application->m_RenderHook.unhook(m_RenderCallback);
		mp_Application->m_PreRenderHook.unhook(m_PreRenderCallback);
		mp_Application->m_EventHook.unhook(m_EventCallback);
		ImGuiModule::get_instance().m_RenderHook.unhook(m_ImGuiRenderCallback);
	}

	void FifedModule::load_default_layout() { ImGui::LoadIniSettingsFromDisk("default_layout.ini"); }

	void FifedModule::on_project_open() { open_editor(); }

	void FifedModule::open_project_manager() { mp_Stage = std::make_unique<ProjectManager>(*this); }

	void FifedModule::open_editor() { mp_Stage = std::make_unique<Editor>(*this); }

	void FifedModule::on_start() {
		ImGuiModule::get_instance().m_RenderHook.hook(m_ImGuiRenderCallback);

		mp_Application->set_pause(true);

		static constexpr ImWchar ranges[] = {0x0020, 0x017f, 0};
		ImGuiIO &io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("assets/fonts/iosevka-regular.ttf", 20, nullptr, ranges);
		io.IniFilename = "layout.ini";

		if(!std::filesystem::exists("layout.ini"))
			load_default_layout();

		mp_Stage = std::make_unique<ProjectManager>(*this);
	}

	void FifedModule::on_update() { mp_Stage->update(); }

	void FifedModule::pre_render() { mp_Stage->pre_render(); }

	void FifedModule::on_render() { mp_Stage->render(); }

	void FifedModule::on_render_imgui() { mp_Stage->render_imgui(); }

	void FifedModule::on_event(Event &event) { mp_Stage->on_event(event); }
}// namespace fifed
