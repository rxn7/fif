#include "fif/gfx/frame_buffer.hpp"

#include "fif/core/opengl.hpp"
#include "fif/core/util/assertion.hpp"

namespace fif::gfx {
	FrameBuffer::FrameBuffer(const glm::vec2 &size) {
		glGenTextures(1, &m_TextureID);
		glGenFramebuffers(1, &m_FboID);

		set_size(size);

		i32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		FIF_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is not complete, status: " << status);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_FboID);
		glDeleteTextures(1, &m_TextureID);
	}

	void FrameBuffer::set_size(const glm::vec2 &size) {
		m_Size = size;

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Size.x, m_Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
		glViewport(0, 0, size.x, size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}// namespace fif::gfx
