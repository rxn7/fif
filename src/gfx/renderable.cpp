#include "fif/gfx/renderable.h"
#include "fif/core/assertion.h"

#include "glad/glad.h"
#include <memory>

namespace fif::gfx {
	Renderable::Renderable(const Vertex *vertices, std::uint32_t vertexCount, const std::uint16_t *elements, std::uint32_t elementCount, const std::shared_ptr<Shader> &shader) : mp_Shader(shader), m_ElementCount(elementCount) {
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(std::uint16_t), elements, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Renderable::render() const {
		mp_Shader->bind();

		glBindVertexArray(m_Vao);
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}
}
