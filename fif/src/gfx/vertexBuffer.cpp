#include "fif/gfx/vertexBuffer.hpp"
#include "fif/core/opengl.hpp"
#include "fif/core/profiler.hpp"
#include "fif/core/util/assertion.hpp"

#include <memory>

namespace fif::gfx {
	VertexBuffer::VertexBuffer(u32 vertexCount, u32 elementCount) : m_VertexCount(vertexCount), m_ElementCount(elementCount) {
		FIF_PROFILE_FUNC();

		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		setupVertexAttributes();

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	VertexBuffer::VertexBuffer(const Vertex *vertices, u32 vertexCount, const u16 *elements, u32 elementCount)
		: m_VertexCount(vertexCount), m_ElementCount(elementCount) {
		FIF_PROFILE_FUNC();

		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		setupVertexAttributes();

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), elements, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void VertexBuffer::render() const {
		FIF_PROFILE_FUNC();

		glBindVertexArray(m_Vao);

		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}

	void VertexBuffer::setupVertexAttributes() {
		FIF_PROFILE_FUNC();

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0u, sizeof(Vertex), reinterpret_cast<const void *>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, 0u, sizeof(Vertex), reinterpret_cast<const void *>(offsetof(Vertex, uv)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, 1u, sizeof(Vertex), reinterpret_cast<const void *>(offsetof(Vertex, color)));
	}

	void VertexBuffer::setVertices(const Vertex *vertices, u32 count) {
		FIF_PROFILE_FUNC();

		m_VertexCount = count;
		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), vertices);
		glBindVertexArray(0);
	}

	void VertexBuffer::setElements(const u16 *elements, u32 count) {
		FIF_PROFILE_FUNC();

		m_ElementCount = count;
		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_ElementCount * sizeof(u16), elements);
		glBindVertexArray(0);
	}
} // namespace fif::gfx
