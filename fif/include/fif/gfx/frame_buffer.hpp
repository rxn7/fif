#pragma once

#include "fif/core/types.hpp"
#include "glm/vec2.hpp"

namespace fif::gfx {
	class FrameBuffer {
	public:
		FrameBuffer(const glm::vec2 &size);
		virtual ~FrameBuffer();

		inline u32 getFboID() const { return m_FboID; }
		inline u32 getTextureID() const { return m_TextureID; }
		void bind();
		void unbind();
		void invalidate();

		void set_size(const glm::vec2 &size);

	private:
		glm::vec2 m_Size;
		u32 m_FboID = 0;
		u32 m_TextureID = 0;
	};
}// namespace fif::gfx
