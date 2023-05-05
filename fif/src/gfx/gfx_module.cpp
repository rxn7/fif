#include "fif/gfx/gfx_module.hpp"

#include "fif/core/event/event_dispatcher.hpp"
#include "fif/core/event/window_event.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shader_library.hpp"
#include "systems/renderer_system.hpp"

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

	glm::vec2 GfxModule::s_ViewportPosition;
	glm::vec2 GfxModule::s_ViewportSize;

	GfxModule::GfxModule() {
		FIF_MODULE_INIT_INSTANCE();

		FIF_LOG("OpenGL Renderer: " << glGetString(GL_RENDERER));
		FIF_LOG("OpenGL Version: " << glGetString(GL_VERSION));
		FIF_LOG("OpenGL Vendor: " << glGetString(GL_VENDOR));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderLibrary::init();
		Renderer2D::init();
	}

	void GfxModule::on_start(core::Application &app) {
		app.add_render_system(&fif::gfx::renderer_system);
	}

	void GfxModule::on_update() {
		glClear(GL_COLOR_BUFFER_BIT);
		Renderer2D::begin();
	}

	void GfxModule::on_render() {
		Renderer2D::end();
	}

	void GfxModule::on_event(fif::core::Event &event) {
		fif::core::EventDispatcher::dispatch<fif::core::WindowResizeEvent>(event, [&](fif::core::WindowResizeEvent &resizeEvent) {
			glViewport(0, 0, resizeEvent.get_size().x, resizeEvent.get_size().y);
			return true;
		});
	}
}// namespace fif::gfx
