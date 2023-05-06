#pragma once

#include "fif/core/module.hpp"
#include "fif/gfx/camera.hpp"
#include "fif/gfx/frame_buffer.hpp"
#include "fif/gfx/renderer2d.hpp"

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		FIF_MODULE(GfxModule)

		GfxModule();

		inline glm::vec2 get_viewport_size() { return m_ViewportSize; }
		inline glm::vec2 get_viewport_position() { return m_ViewportPosition; }
		inline glm::vec2 get_point_relative_to_viewport(const glm::vec2 &point) { return point - m_ViewportPosition; }

		inline Renderer2D &get_renderer2D() { return m_Renderer2D; }

		inline void set_viewport(const glm::vec2 &size, const glm::vec2 &pos = {0, 0}) {
			m_ViewportSize = size;
			m_ViewportPosition = pos;
		}

		void on_start(core::Application &app) override;
		void on_update() override;
		void on_render() override;
		void on_event(core::Event &event) override;

	public:
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportPosition;

	private:
		Renderer2D m_Renderer2D;
	};
}// namespace fif::gfx
