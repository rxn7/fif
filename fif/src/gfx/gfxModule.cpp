#include "fif/gfx/gfxModule.hpp"
#include "fif/core/application.hpp"
#include "fif/core/event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/gfx/renderer2d.hpp"
#include "fif/gfx/shaderLibrary.hpp"

#include <memory>

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

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

	void GfxModule::onUpdate() {
		glClear(GL_COLOR_BUFFER_BIT);
		Renderer2D::begin();
	}

	void GfxModule::onRender() {
		Renderer2D::end();
	}
} // namespace fif::gfx
