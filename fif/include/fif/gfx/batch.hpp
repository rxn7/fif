#pragma once

#include "fif/gfx/camera.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer.hpp"

namespace fif::gfx {
	template<typename Vertex> class Batch {
	public:
		Batch(u32 verticesPerInstance, u32 elementsPerInstance, u32 size, const VertexBufferLayout &layout) :
			m_MaxElements(elementsPerInstance * size), m_MaxVertices(verticesPerInstance * size), mp_Vertices(new Vertex[m_MaxVertices]), mp_Elements(new u16[m_MaxElements]), m_Buffer(m_MaxVertices, m_MaxElements, sizeof(Vertex)) {
			m_Buffer.set_layout(layout);
		}

		inline u32 get_vertex_count() const { return m_VertexCount; }
		inline u32 get_element_count() const { return m_ElementCount; }
		inline bool is_full() const { return m_VertexCount >= m_MaxVertices; }
		inline bool is_empty() const { return m_VertexCount == 0; }

		void flush() {
			m_Buffer.set_vertices_and_elements(mp_Vertices, m_VertexCount, mp_Elements, m_ElementCount);
			m_Buffer.render();

			m_VertexCount = m_ElementCount = 0;
		}

		void add_vertex(const Vertex &vertex) {
			if(m_VertexCount >= m_MaxVertices) {
				core::Logger::error("The batch's vertex buffer is full: [%u/%u]", m_VertexCount, m_MaxVertices);
				return;
			}

			mp_Vertices[m_VertexCount++] = vertex;
		}

		void add_element(u16 element) {
			if(m_ElementCount >= m_MaxElements) {
				core::Logger::error("The batch's element buffer is full: [%u/%u]", m_ElementCount, m_MaxElements);
				return;
			}

			mp_Elements[m_ElementCount++] = element;
		}

	private:
		u32 m_ElementCount, m_MaxElements;
		u32 m_VertexCount, m_MaxVertices;

		Vertex *mp_Vertices;
		u16 *mp_Elements;

		VertexBuffer m_Buffer;
	};
}// namespace fif::gfx
