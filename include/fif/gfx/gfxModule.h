#pragma once

#include "fif/core/module.h"
#include "fif/gfx/camera.h"

#include <memory>

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		constexpr std::string_view getName() const override { return "GFX"; }
		void onAttach(core::Application &app) override;
		void onDetach() override;
		void onEvent(core::Event &event) override;
		void update(float dt) override;
		void render() override;
	};
}
