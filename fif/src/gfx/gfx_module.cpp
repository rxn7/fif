#include "fif/gfx/gfx_module.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "systems/renderer_system.hpp"

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

	GfxModule::GfxModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	void GfxModule::on_start() {
		core::Logger::info("OpenGL Version: %s", glGetString(GL_VERSION));
		core::Logger::info("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
		core::Logger::info("OpenGL Renderer: %s", glGetString(GL_RENDERER));
		core::Logger::info("OpenGL Vendor: %s", glGetString(GL_VENDOR));

		mp_Application->add_render_system(&fif::gfx::renderer_system);
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
