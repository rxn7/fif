#include "entt/signal/fwd.hpp"
#include "fif/core/application.h"
#include "fif/core/events/renderEvent.h"
#include "fif/core/module.h"
#include "fif/gfx/gfx.h"
#include "fif/gfx/shaderLibrary.h"
#include "fif/gfx/renderer2d.h"

#include "fif/core/events/updateEvent.h"

#include "entt/entt.hpp"
#include "glad/glad.h"

#include <memory>

namespace fif::gfx {
	static std::shared_ptr<Camera> s_Camera;

	Camera &getCamera() {
		return *s_Camera;
	}

	void init() {
		s_Camera = std::make_shared<Camera>();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderLibrary::init();
		Renderer2D::init();
	}

	void update(float dt) {
		s_Camera->update();
		Renderer2D::begin(s_Camera);
	}

	void render() {
		Renderer2D::end();
	}
}
