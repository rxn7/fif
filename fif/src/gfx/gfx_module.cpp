#include "fif/gfx/gfx_module.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader_library.hpp"
#include "systems/renderer_system.hpp"

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

	GfxModule::GfxModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	void GfxModule::on_start(core::Application &app) {
		app.add_render_system(&fif::gfx::renderer_system);
	}

	void GfxModule::on_update() {
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
