#pragma once

#include "fif/gfx/camera.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer.hpp"

namespace fif::gfx {
	template<typename Vertex> class Batch {
	public:
		Batch(u32 verticesPerInstance, u32 elementsPerInstance, u32 size, const VertexBufferLayout &layout) :
			m_Vertices(verticesPerInstance * size), m_Elements(elementsPerInstance * size), m_Buffer(verticesPerInstance * size, elementsPerInstance * size, sizeof(Vertex)) {
			m_Buffer.set_layout(layout);
		}

		inline u32 get_vertex_count() const { return m_VertexCount; }
		inline u32 get_element_count() const { return m_ElementCount; }

		void flush() {
			m_Buffer.set_vertices_and_elements(m_Vertices.data(), m_VertexCount, m_Elements.data(), m_ElementCount);
			m_Buffer.render();

			m_VertexCount = m_ElementCount = 0;
		}

		void add_vertex(const Vertex &vertex) { m_Vertices[m_VertexCount++] = vertex; }

		void add_element(u16 element) { m_Elements[m_ElementCount++] = element; }

	private:
		u32 m_VertexCount;
		u32 m_ElementCount;

		std::vector<Vertex> m_Vertices;
		std::vector<u16> m_Elements;

		VertexBuffer m_Buffer;
	};
}// namespace fif::gfx
