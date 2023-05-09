#pragma once

#include "fif/core/types.hpp"
#include "fif/gfx/texture.hpp"

namespace fif::gfx {
	class FrameBuffer {
	public:
		FrameBuffer(const glm::vec2 &size);
		virtual ~FrameBuffer();

		inline u32 getFboID() const { return m_FboID; }
		inline const Texture &getTexture() const { return m_Texture; }

		void start();
		void end();

		void bind();
		void unbind();
		void invalidate();

		void set_size(const glm::vec2 &size);
		inline void set_clear_color(const Color &color) { m_Color = color; }

	private:
		Color m_Color = {200u, 200u, 200u, 255u};
		Texture m_Texture;
		glm::vec2 m_Size;
		u32 m_FboID = 0;
	};
}// namespace fif::gfx
