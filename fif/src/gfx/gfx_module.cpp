#include "./lua_register.hpp"
#include "./serialization/gfx_entity_serializer.hpp"
#include "./systems/renderer_system.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/core/resource/resource_manager.hpp"
#include "fif/core/serialization/scene_serializer.hpp"
#include "fif/gfx/gfx_module.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/resource/font.hpp"

#include <memory>

namespace fif::gfx {
	GfxModule::GfxModule(const std::filesystem::path &defaultFontPath) :
		m_StartCallback(std::bind(&GfxModule::on_start, this)), m_PreRenderCallback(std::bind(&GfxModule::pre_render, this)), m_RenderCallback(std::bind(&GfxModule::on_render, this)), m_EndFrameCallback(std::bind(&GfxModule::end_frame, this)), m_EventCallback(std::bind(&GfxModule::on_event, this, std::placeholders::_1)) {
		FIF_MODULE_INIT();

		core::Application &app = core::Application::get_instance();
		app.m_StartHook.hook(m_StartCallback);
		app.m_PreRenderHook.hook(m_PreRenderCallback);
		app.m_RenderHook.hook(m_RenderCallback);
		app.m_EndFrameHook.hook(m_EndFrameCallback);
		app.m_EventHook.hook(m_EventCallback);

		FIF_ASSERT(FT_Init_FreeType(&m_FreeType) == 0, "Failed to init freetype");

		// Texture loader
		core::ResourceManager::add_loader_func(
			Texture::get_type_static(), [](const std::filesystem::path &path) -> std::shared_ptr<core::Resource> { return std::dynamic_pointer_cast<core::Resource>(std::make_shared<Texture>(path)); });

		// Font loader
		core::ResourceManager::add_loader_func(
			Font::get_type_static(), [](const std::filesystem::path &path) -> std::shared_ptr<core::Resource> { return std::dynamic_pointer_cast<core::Resource>(std::make_shared<Font>(path)); });

		Font::sp_DefaultFont = std::make_unique<Font>(defaultFontPath, 32, 512);
	}

	GfxModule::~GfxModule() {
		mp_Application->m_StartHook.unhook(m_StartCallback);
		mp_Application->m_PreRenderHook.unhook(m_PreRenderCallback);
		mp_Application->m_RenderHook.unhook(m_RenderCallback);
		mp_Application->m_EndFrameHook.hook(m_EndFrameCallback);
		mp_Application->m_EventHook.unhook(m_EventCallback);

		// NOTE: We need to delete it now, or else the font's texture will be deleted after the OpenGL context is destroyed, which causes a segfault.
		Font::sp_DefaultFont.reset();

		FT_Done_FreeType(m_FreeType);
	}

	void GfxModule::on_start() {
		register_lua_types();

		core::Logger::info("OpenGL Version: %s", glGetString(GL_VERSION));
		core::Logger::info("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		core::Logger::info("OpenGL Renderer: %s", glGetString(GL_RENDERER));
		core::Logger::info("OpenGL Vendor: %s", glGetString(GL_VENDOR));

		mp_Application->add_render_system(&fif::gfx::renderer_system);

		core::SceneSerializer::add_serializer<GfxEntitySerializer>();
	}

	void GfxModule::pre_render() {
		glClear(GL_COLOR_BUFFER_BIT);
		m_Renderer2D.start();
	}

	void GfxModule::on_render() {
		m_Renderer2D.end();
		m_Renderer2D.start_ui();
		m_UIRenderCallback.invoke();
	}

	void GfxModule::end_frame() {
		m_Renderer2D.end_ui();
	}

	void GfxModule::on_event(fif::core::Event &event) {
		fif::core::EventDispatcher::dispatch<fif::core::WindowResizeEvent>(event, [&](fif::core::WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.get_size().x, resizeEvent.get_size().y);
			return true;
		});
	}
}// namespace fif::gfx
