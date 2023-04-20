#pragma once

#include "fif/core/module.h"
#include "fif/gfx/camera.h"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	  public:
		FIF_MODULE_NAME(GFX)

		GfxModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(GfxModule)

		void onUpdate() override;
		void onRender() override;
	};
} // namespace fif::gfx
