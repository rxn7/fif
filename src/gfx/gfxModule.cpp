#include "entt/signal/fwd.hpp"
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

	void GfxModule::onAttach([[maybe_unused]] core::Application &app) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderLibrary::init();
		Renderer2D::init();
	}

	void GfxModule::onDetach() {
	}

	void GfxModule::update([[maybe_unused]] float dt) {
		glClear(GL_COLOR_BUFFER_BIT);
		Renderer2D::begin();
	}

	void GfxModule::render() {
		Renderer2D::end();
	}

	void GfxModule::onEvent([[maybe_unused]] core::Event &event) {
	}
}
