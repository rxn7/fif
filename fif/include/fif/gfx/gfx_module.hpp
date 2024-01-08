#pragma once

#include "fif/core/invokable.hpp"
#include "fif/core/module.hpp"
#include "fif/gfx/camera.hpp"
#include "fif/gfx/frame_buffer.hpp"
#include "fif/gfx/renderer2d.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace fif::gfx {
	class GfxModule final : public core::Module {
	public:
		FIF_MODULE(GfxModule)

		GfxModule(const std::filesystem::path &defaultFontPath);
		~GfxModule();

		inline vec2 get_viewport_size() { return m_ViewportSize; }
		inline vec2 get_viewport_position() { return m_ViewportPosition; }

		inline vec2 get_point_relative_to_viewport(const vec2 &point) { return point - m_ViewportPosition; }

		inline Renderer2D &get_renderer2D() { return m_Renderer2D; }

		inline void set_viewport(const vec2 &size, const vec2 &pos = {0, 0}) {
			m_ViewportSize = size;
			m_ViewportPosition = pos;
		}

	private:
		void on_start();
		void pre_render();
		void on_render();
		void end_frame();
		void on_event(core::Event &event);

	public:
		vec2 m_ViewportSize;
		vec2 m_ViewportPosition;
		FT_Library m_FreeType;

	private:
		Invokable<> m_UIRenderCallback;
		Callback<> m_StartCallback;
		Callback<> m_PreRenderCallback;
		Callback<> m_RenderCallback;
		Callback<> m_EndFrameCallback;
		Callback<core::Event &> m_EventCallback;

		Renderer2D m_Renderer2D;
	};
}// namespace fif::gfx
