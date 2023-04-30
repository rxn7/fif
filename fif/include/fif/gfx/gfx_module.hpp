#pragma once

#include "fif/core/module.hpp"
#include "fif/gfx/camera.hpp"
#include "fif/gfx/frame_buffer.hpp"

#include "glm/vec2.hpp"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		FIF_MODULE_NAME(GFX)

		GfxModule();
		FIF_MODULE_INSTANCE_FUNC_DECL(GfxModule)

		static inline glm::vec2 get_viewport_size() { return s_ViewportSize; }
		static inline glm::vec2 get_viewport_position() { return s_ViewportPosition; }
		static inline glm::vec2 get_point_relative_to_viewport(const glm::vec2 &point) { return point - s_ViewportPosition; }

		static inline void set_viewport(const glm::vec2 &size, const glm::vec2 &pos = {0, 0}) {
			s_ViewportSize = size;
			s_ViewportPosition = pos;
		}

		void on_start(core::Application &app) override;
		void on_update() override;
		void on_render() override;
		void on_event(core::Event &event) override;

	public:
		static glm::vec2 s_ViewportSize;
		static glm::vec2 s_ViewportPosition;
	};
}// namespace fif::gfx
