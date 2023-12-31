#include "fif/gfx/gfx_module.hpp"

#include "fif/core/ecs/serialization/scene_serializer.hpp"
#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/gfx/renderer2d.hpp"

#include "./lua_register.hpp"
#include "./serialization/gfx_serializer.hpp"
#include "./systems/renderer_system.hpp"

#include <memory>

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

	GfxModule::GfxModule(const std::string &defaultFontPath) {
		FIF_MODULE_INIT_INSTANCE();

		FIF_ASSERT(FT_Init_FreeType(&m_FreeType) == 0, "Failed to init freetype");

		mp_DefaultFont = std::make_shared<Font>(defaultFontPath, 64);
	}

	GfxModule::~GfxModule() {
		FT_Done_FreeType(m_FreeType);
	}

	void GfxModule::on_start() {
		register_lua_components();

		core::Logger::info("OpenGL Version: %s", glGetString(GL_VERSION));
		core::Logger::info("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		core::Logger::info("OpenGL Renderer: %s", glGetString(GL_RENDERER));
		core::Logger::info("OpenGL Vendor: %s", glGetString(GL_VENDOR));

		mp_Application->add_render_system(&fif::gfx::renderer_system);

		core::SceneSerializer::add_serializer<GfxSerializer>();
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
