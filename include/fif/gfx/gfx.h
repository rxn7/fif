#pragma once

#include "fif/core/module.h"

namespace fif::gfx {
	void init();
	void update(float dt);
	void render();

	inline core::Module gfxModule = {
		.initFunc = fif::gfx::init,
		.updateFunc = fif::gfx::update,
		.renderFunc = fif::gfx::render
	};
}
