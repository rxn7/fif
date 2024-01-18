#include "fif/gfx/frame_buffer.hpp"

namespace fif::gfx {
	FrameBuffer::FrameBuffer(const vec2 &size) : mp_Texture(std::make_shared<Texture>(size, GL_NEAREST)), mp_EntityIdTexture(std::make_shared<Texture>(size, GL_NEAREST)) {
		glGenFramebuffers(1, &m_FboID);
		set_size(size);

		i32 status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		FIF_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is not complete, status: %d", status);

		unbind();
	}

	FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &m_FboID); }

	void FrameBuffer::start() {
		bind();
		const vec3 clearColor = gfx::normalize_color3(m_ClearColor);

		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		const u32 invalidId = std::numeric_limits<u32>::max();
		glClearBufferuiv(GL_COLOR, 1, &invalidId);
	}

	void FrameBuffer::end() { unbind(); }

	void FrameBuffer::invalidate() {
		mp_Texture->create(m_Size, GL_RGB8, GL_RGB, GL_NEAREST, GL_CLAMP_TO_EDGE, nullptr);
		mp_EntityIdTexture->create(m_Size, GL_R32UI, GL_RED_INTEGER, GL_NEAREST, GL_CLAMP_TO_EDGE, nullptr);

		bind();

		glViewport(0, 0, m_Size.x, m_Size.y);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mp_Texture->get_id(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mp_EntityIdTexture->get_id(), 0);

		constexpr std::array<u32, 2> attachments{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(attachments.size(), attachments.data());

		unbind();
	}

	void FrameBuffer::set_size(const vec2 &size) {
		m_Size = size;
		invalidate();
	}

	u32 FrameBuffer::read_entity_id_buffer_pixel(const u32vec2 &point) const {
		bind();
		glReadBuffer(GL_COLOR_ATTACHMENT1);

		u32 clickedId;
		glReadPixels(point.x, point.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &clickedId);

		unbind();

		return clickedId;
	}
}// namespace fif::gfx
