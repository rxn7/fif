#include "fif/gfx/vertex_buffer.hpp"
#include "fif/gfx/glsl_data_type.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	VertexBuffer::VertexBuffer(const u32 vertexCount, const u32 elementCount, const u32 vertexSize, const VertexBufferLayout &layout) :
		m_Layout(layout), m_VertexCount(vertexCount), m_ElementCount(elementCount), m_VertexSize(vertexSize) {
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * m_VertexSize, nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);

		setup_vertex_attributes();

		glBindVertexArray(0);
	}

	VertexBuffer::VertexBuffer(const void *vertices, const u32 vertexCount, const u16 *elements, const u32 elementCount, const u32 vertexSize, const VertexBufferLayout &layout) :
		m_Layout(layout), m_VertexCount(vertexCount), m_ElementCount(elementCount), m_VertexSize(vertexSize) {
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * m_VertexSize, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), elements, GL_STATIC_DRAW);

		setup_vertex_attributes();

		glBindVertexArray(0);
	}

	void VertexBuffer::render() const {
		glBindVertexArray(m_Vao);
		glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(0);
	}

	void VertexBuffer::setup_vertex_attributes() {
		u8 i = 0;
		for(const VertexBufferElement &el : m_Layout.get_elements()) {
			glEnableVertexAttribArray(i);

			if(el.asFloat)
				glVertexAttribPointer(i, get_glsl_data_type_component_count(el.type), glsl_data_type_to_opengl_enum(el.type), el.normalized, m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));
			else
				glVertexAttribIPointer(i, get_glsl_data_type_component_count(el.type), glsl_data_type_to_opengl_enum(el.type), m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));

			++i;
		}
	}

	void VertexBuffer::set_vertices_and_elements(const void *vertices, const u32 vertexCount, const u16 *elements, const u32 elementCount) {
		m_VertexCount = vertexCount;
		m_ElementCount = elementCount;
		glBindVertexArray(m_Vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * m_VertexSize, vertices);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_ElementCount * sizeof(u16), elements);

		glBindVertexArray(0);
	}
}// namespace fif::gfx
