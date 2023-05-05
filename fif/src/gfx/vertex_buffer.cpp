#include "fif/gfx/vertex_buffer.hpp"

namespace fif::gfx {
	VertexBuffer::VertexBuffer(u32 vertexCount, u32 elementCount, u32 vertexSize) :
		m_VertexCount(vertexCount), m_ElementCount(elementCount), m_VertexSize(vertexSize) {
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * m_VertexSize, nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	VertexBuffer::VertexBuffer(const void *vertices, u32 vertexCount, const u16 *elements, u32 elementCount, u32 vertexSize) :
		m_VertexCount(vertexCount), m_ElementCount(elementCount), m_VertexSize(vertexSize) {
		glGenVertexArrays(1, &m_Vao);
		glBindVertexArray(m_Vao);

		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * m_VertexSize, vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_Ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount * sizeof(u16), elements, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	void VertexBuffer::set_layout(const VertexBufferLayout &layout) {
		glBindVertexArray(m_Vao);
		m_Layout = layout;
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

			switch(el.type) {
			case GlslDataType::Float:
			case GlslDataType::Float2:
			case GlslDataType::Float3:
			case GlslDataType::Float4:
				glVertexAttribPointer(i, get_glsl_data_type_component_count(el.type), GL_FLOAT, el.normalized, m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));
				break;

			case GlslDataType::Int:
			case GlslDataType::Int2:
			case GlslDataType::Int3:
			case GlslDataType::Int4:
				glVertexAttribPointer(i, get_glsl_data_type_component_count(el.type), GL_INT, el.normalized, m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));
				break;

			case GlslDataType::UByte:
			case GlslDataType::UByte2:
			case GlslDataType::UByte3:
			case GlslDataType::UByte4:
				glVertexAttribPointer(i, get_glsl_data_type_component_count(el.type), GL_UNSIGNED_BYTE, el.normalized, m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));
				break;

			case GlslDataType::Bool:
				glVertexAttribPointer(i, get_glsl_data_type_component_count(el.type), GL_BOOL, el.normalized, m_Layout.get_stride(), reinterpret_cast<void *>(el.offset));
				break;

			default:
				FIF_LOG_ERROR("Unsupported GlslDataType for VertexBufferElement: " << (int)el.type);
				break;
			};

			++i;
		}
	}

	void VertexBuffer::set_vertices_and_elements(const void *vertices, u32 vertexCount, const u16 *elements, u32 elementCount) {
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
