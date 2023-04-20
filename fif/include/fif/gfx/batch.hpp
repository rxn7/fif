#pragma once

#include "fif/core/types.hpp"
#include "fif/core/util/assertion.hpp"
#include "fif/gfx/camera.hpp"
#include "fif/gfx/shader.hpp"
#include "fif/gfx/vertex.hpp"
#include "fif/gfx/vertexBuffer.hpp"

#include <vector>

namespace fif::gfx {
	class Batch {
	  public:
		Batch(u32 verticesPerInstance, u32 elementsPerInstance, u32 size);
		void render();

		inline u32 getVertexCount() const { return m_Vertices.size(); }

		inline u32 getElementCount() const { return m_Elements.size(); }

		inline void addVertex(const Vertex &vertex) {
			FIF_ASSERT(m_Vertices.size() != m_Vertices.capacity(),
					   "Cannot add vertex, the buffer is full");
			m_Vertices.push_back(vertex);
		}

		inline void addElement(u16 element) {
			FIF_ASSERT(m_Elements.size() != m_Elements.capacity(),
					   "Cannot add element, the buffer is full");
			m_Elements.push_back(element);
		}

	  private:
		VertexBuffer m_Buffer;
		std::vector<Vertex> m_Vertices;
		std::vector<u16> m_Elements;
	};
} // namespace fif::gfx
