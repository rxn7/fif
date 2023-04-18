#include "fif/core/application.h"
#include "fif/core/assertion.h"
#include "fif/core/event/event.h"
#include "fif/core/module.h"
#include "fif/gfx/gfxModule.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/renderer2d.h"

#include "fif/core/opengl.h"

#include <memory>

namespace fif::gfx {
	FIF_MODULE_INSTANCE_IMPL(GfxModule);

	GfxModule::GfxModule() {
		FIF_MODULE_INIT_INSTANCE();
	}

	void GfxModule::onStart([[maybe_unused]] core::Application &app) {
		FIF_LOG("OpenGL Renderer: " << glGetString(GL_RENDERER));
		FIF_LOG("OpenGL Version: " << glGetString(GL_VERSION));
		FIF_LOG("OpenGL Vendor: " << glGetString(GL_VENDOR));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderLibrary::init();
		Renderer2D::init();
	}

	void GfxModule::onUpdate([[maybe_unused]] float dt) {
		glClear(GL_COLOR_BUFFER_BIT);
		Renderer2D::begin();
	}

	void GfxModule::onRender() {
		Renderer2D::end();
	}
}
