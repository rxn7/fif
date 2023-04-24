#pragma once

#include "event/event.hpp"
#include "fif/core/module.hpp"
#include "fif/gfx/camera.hpp"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	  public:
		FIF_MODULE_NAME(GFX)

		GfxModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(GfxModule)

		void onUpdate() override;
		void onRender() override;
		void onEvent(core::Event &event) override;
	};
} // namespace fif::gfx
