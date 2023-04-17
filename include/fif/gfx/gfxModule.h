#pragma once

#include "fif/core/module.h"
#include "fif/gfx/camera.h"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		GfxModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(GfxModule)

		constexpr std::string_view getName() const override { return "GFX"; }

		void onStart(core::Application &app) override;
		void onUpdate(float dt) override;
		void onRender() override;
	};
}
