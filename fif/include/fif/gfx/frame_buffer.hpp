#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/color.hpp"
#include "fif/gfx/resource/texture.hpp"

namespace fif::gfx {
	class FrameBuffer final {
	public:
		FrameBuffer(const vec2 &size);
		~FrameBuffer();

		inline u32 get_fbo_id() const { return m_FboID; }
		inline const Texture &get_texture() const { return m_Texture; }

		void start();
		void end();

		void bind();
		void unbind();
		void invalidate();

		void set_size(const vec2 &size);
		inline const vec2 &get_size() const { return m_Size; }

	public:
		Color3 m_Color = {200u, 200u, 200u};

	private:
		Texture m_Texture;
		vec2 m_Size;
		u32 m_FboID = 0;
	};
}// namespace fif::gfx
