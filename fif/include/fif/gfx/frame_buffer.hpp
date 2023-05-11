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

	public:
		Color3 m_Color = {200u, 200u, 200u};

	private:
		Texture m_Texture;
		glm::vec2 m_Size;
		u32 m_FboID = 0;
	};
}// namespace fif::gfx
