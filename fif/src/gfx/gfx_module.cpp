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
	GfxModule::GfxModule(const std::filesystem::path &defaultFontPath) {
		FIF_MODULE_INIT();
		FIF_ASSERT(FT_Init_FreeType(&m_FreeType) == 0, "Failed to init freetype");

		core::ResourceManager::add_loader_func(
			Texture::get_type_static(), [](const std::filesystem::path &path) -> std::shared_ptr<core::Resource> { return std::dynamic_pointer_cast<core::Resource>(std::make_shared<Texture>(path)); });

		core::ResourceManager::add_loader_func(
			Font::get_type_static(), [](const std::filesystem::path &path) -> std::shared_ptr<core::Resource> { return std::dynamic_pointer_cast<core::Resource>(std::make_shared<Font>(path)); });

		Font::sp_DefaultFont = std::make_shared<Font>(defaultFontPath, 64, 512);
	}

	GfxModule::~GfxModule() {
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
	}

	void GfxModule::on_event(fif::core::Event &event) {
		fif::core::EventDispatcher::dispatch<fif::core::WindowResizeEvent>(event, [&](fif::core::WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.get_size().x, resizeEvent.get_size().y);
			return true;
		});
	}
}// namespace fif::gfx
