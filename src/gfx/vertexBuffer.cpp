#include "fif/gfx/vertexBuffer.h"
#include "fif/core/assertion.h"

#include "glad/glad.h"
#include <memory>

namespace fif::gfx {
	VertexBuffer::VertexBuffer(std::uint32_t vertexCount, std::uint32_t elementCount) : m_ElementCount(elementCount), m_VertexCount(vertexCount)  {
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		setupVertexAttributes();

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(std::uint16_t), nullptr, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	VertexBuffer::VertexBuffer(const Vertex *vertices, std::uint32_t vertexCount, const std::uint16_t *elements, std::uint32_t elementCount) : m_ElementCount(elementCount), m_VertexCount(vertexCount) {
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		setupVertexAttributes();

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(std::uint16_t), elements, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void VertexBuffer::render() const {
		glBindVertexArray(m_Vao);

		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}

	void VertexBuffer::setupVertexAttributes() {
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color)));
	}

	void VertexBuffer::setVertices(const Vertex *vertices, std::uint32_t count) {
		m_VertexCount = count;
		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), vertices);
		glBindVertexArray(0);
	}

	void VertexBuffer::setElements(const uint16_t *elements, std::uint32_t count) {
		m_ElementCount = count;
		glBindVertexArray(m_Vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_ElementCount * sizeof(std::uint16_t), elements);
		glBindVertexArray(0);
	}
}
