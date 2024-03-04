#pragma once

#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer_layout.hpp"

namespace fif::gfx {
	template<typename Vertex> class VertexBuffer final {
	public:
		VertexBuffer(const std::span<Vertex const> vertices, const std::span<u16 const> elements, const VertexBufferLayout &layout) :
			m_Layout(layout), m_VertexCount(vertices.size()), m_ElementCount(elements.size()) {
			glGenVertexArrays(1, &m_Vao);
			glBindVertexArray(m_Vao);

			glGenBuffers(1, &m_Vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
			glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &m_Ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size_bytes(), elements.data(), GL_STATIC_DRAW);

			setup_vertex_attributes();

			glBindVertexArray(0);
		}

		VertexBuffer(const u32 vertexCount, const u32 elementCount, const u32 m_VertexSize, const VertexBufferLayout &layout) :
			m_Layout(layout), m_VertexCount(vertexCount), m_ElementCount(elementCount) {
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

		void render() const {
			glBindVertexArray(m_Vao);
			glDrawElements(GL_TRIANGLES, m_ElementCount, GL_UNSIGNED_SHORT, 0);

			glBindVertexArray(0);
		}

		void set_vertices_and_elements(const std::span<Vertex const> vertices, const std::span<u16 const> elements) {
			m_VertexCount = vertices.size();
			m_ElementCount = elements.size();
			glBindVertexArray(m_Vao);

			glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(Vertex), vertices.data());

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_ElementCount * sizeof(u16), elements.data());

			glBindVertexArray(0);
		}

		const VertexBufferLayout &get_layout() const { return m_Layout; }

	private:
		void setup_vertex_attributes() {
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

	protected:
		const VertexBufferLayout &m_Layout;
		u32 m_VertexCount;
		u32 m_ElementCount;
		u32 m_Vao;
		u32 m_Vbo;
		u32 m_Ebo;
	};
}// namespace fif::gfx
