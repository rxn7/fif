#include "fif/gfx/frame_buffer.hpp"

namespace fif::gfx {
	FrameBuffer::FrameBuffer(const glm::vec2 &size) : m_Texture(size.x, size.y, GL_NEAREST) {
		glGenFramebuffers(1, &m_FboID);

		set_size(size);

		i32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		FIF_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is not complete, status: " << status);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &m_FboID);
	}

	void FrameBuffer::set_size(const glm::vec2 &size) {
		m_Size = size;

		m_Texture.bind();
		m_Texture.create(size.x, size.y, GL_RGB, GL_RGB, GL_NEAREST, GL_CLAMP_TO_EDGE, nullptr);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
		glViewport(0, 0, size.x, size.y);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.getID(), 0);

		Texture::unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FboID);
	}

	void FrameBuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}// namespace fif::gfx
