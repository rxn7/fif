#include "entt/signal/fwd.hpp"
#include "fif/core/application.h"
#include "fif/core/module.h"
#include "fif/gfx/gfx.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/renderer2d.h"

#include "fif/core/opengl.h"

#include <memory>

namespace fif::gfx {
	void init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderLibrary::init();
		Renderer2D::init();
	}

	void update([[maybe_unused]] float dt) {
		Renderer2D::begin();
	}

	void render() {
		Renderer2D::end();
	}
}
