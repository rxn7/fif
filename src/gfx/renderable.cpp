#include "fif/gfx/renderable.h"
#include "fif/core/assertion.h"

#include "glad/glad.h"
#include <memory>

namespace fif::gfx {
	Renderable::Renderable(const std::vector<Vertex> &vertices, const std::vector<std::uint16_t> &elements, const std::shared_ptr<Shader> &shader) : mp_Shader(shader), m_ElementCount(elements.size()) {
		glCreateVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, uv)));

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(std::uint16_t), elements.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void Renderable::render() const {
		mp_Shader->bind();

		glBindVertexArray(m_Vao);
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}
}
