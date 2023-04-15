#pragma once

#include "fif/core/module.h"
#include "fif/gfx/camera.h"

#include <memory>

namespace fif::gfx {
	void init();
	void update(float dt);
	void render();

	inline core::Module gfxModule = {
		.name = "GFX",
		.initFunc = fif::gfx::init,
		.updateFunc = fif::gfx::update,
		.renderFunc = fif::gfx::render
	};
}
