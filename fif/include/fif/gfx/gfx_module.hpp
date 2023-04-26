#pragma once

#include "application.hpp"
#include "fif/core/module.hpp"
#include "fif/gfx/camera.hpp"
#include "fif/gfx/frame_buffer.hpp"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		FIF_MODULE_NAME(GFX)

		GfxModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(GfxModule)

		void on_update() override;
		void on_render() override;
		void on_event(core::Event &event) override;
	};
}// namespace fif::gfx
