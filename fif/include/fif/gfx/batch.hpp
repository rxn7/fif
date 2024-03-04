#pragma once

#include "fif/gfx/camera.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex_buffer.hpp"

namespace fif::gfx {
	template<typename Vertex> class Batch {
	public:
		Batch(const u32 verticesPerInstance, const u32 elementsPerInstance, const u32 size, const VertexBufferLayout &layout, const std::string &shaderVert, const std::string &shaderFrag) :
			m_MaxElements(elementsPerInstance * size), m_MaxVertices(verticesPerInstance * size), m_Vertices(m_MaxVertices), m_Elements(m_MaxElements), m_Buffer(m_MaxVertices, m_MaxElements, sizeof(Vertex), layout), m_Shader(shaderVert, shaderFrag) {}

		virtual ~Batch() {}

		inline Shader &get_shader() { return m_Shader; }
		inline u32 get_vertex_count() const { return m_VertexCount; }
		inline u32 get_element_count() const { return m_ElementCount; }

		inline bool is_full() const { return m_VertexCount >= m_MaxVertices; }
		inline bool can_fit(const u32 vertexCount, const u32 elementCount) const { return m_VertexCount + vertexCount <= m_MaxVertices && m_ElementCount + elementCount <= m_MaxElements; }
		inline bool is_empty() const { return m_VertexCount == 0; }

		void flush() {
			if(m_VertexCount == 0)
				return;

			m_Buffer.set_vertices_and_elements(m_Vertices, m_Elements);
			m_Buffer.render();

			m_VertexCount = m_ElementCount = 0;
		}

		void add_vertex(const Vertex &vertex) {
			if(m_VertexCount >= m_MaxVertices) {
				core::Logger::error("The batch's vertex buffer is full: [%u/%u]", m_VertexCount, m_MaxVertices);
				return;
			}

			m_Vertices[m_VertexCount++] = vertex;
		}

		void add_element(u16 element) {
			if(m_ElementCount >= m_MaxElements) {
				core::Logger::error("The batch's element buffer is full: [%u/%u]", m_ElementCount, m_MaxElements);
				return;
			}

			m_Elements[m_ElementCount++] = element;
		}

	private:
		u32 m_ElementCount = 0, m_MaxElements;
		u32 m_VertexCount = 0, m_MaxVertices;

		std::vector<Vertex> m_Vertices;
		std::vector<u16> m_Elements;

		VertexBuffer<Vertex> m_Buffer;
		Shader m_Shader;

		friend class Renderer2D;
	};
}// namespace fif::gfx
